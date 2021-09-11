/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

static const float PI = 3.14f;
static const int SpotLightNum = 2;

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

/*PBR用のライトデータ*/
struct DirectionalLight
{
    float3 direction;
    float3 color;
};

struct SpotLight
{
    float3 spotPosition;
    float3 spotColor;
    float spotRange;
    float3 spotDirection;
    float spotAngle;
};

//ライトビュープロジェクション行列にアクセスする定数バッファを定義。
cbuffer ShadowCb : register(b1)
{
    float4x4 LVP;
    float4x4 viewProjInverseMatrix;
	/***** p b r *********************/
    DirectionalLight directionalLight;

    float3 eyePos;
    float specPow;
    float3 ambientLight;
	/**************************/

	/*スポットライト用*********/
    SpotLight spotLight[2];
	/**************************/
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> normalTexture : register(t1);
Texture2D<float4> specDepthTexture : register(t2);
Texture2D<float4> shadowMap : register(t3);

sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

/****************ここからPBR関数*********************************************************************/

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
    float3 binSpaceNormal = normalTexture.SampleLevel(Sampler, uv, 0.0f).xyz;
    binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

    float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

    return newNormal;
}

//ベックマン分布を計算する
//表面の微小な傾き下限の値。
float Beckmann(float m, float t)
{
    float t2 = t * t;
    float t4 = t * t * t * t;
    float m2 = m * m;
    float D = 1.0f / (4.0f * m2 * t4);
    D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
    return D;
}

//フレネルを計算。Schlick近似を使用
//光の波長と入射角によって金属反射波きまるらしい。それの計算
float SpcFresnel(float f0, float u)
{
	// from Schlick
    return f0 + (1 - f0) * pow(1 - u, 5);
}

//引数は法線、-ディレクションライトの方向、目への方向
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    //ライトとカメラのハーフベクトル
    float3 H = normalize(L + V);
    //物体の粗さ
    float roughness = 0.5f;
    float energyBias = lerp(0.0f, 0.5f, roughness);
    //ライトとハーフベクトルの内積
    float dotLH = saturate(dot(L, H));
    //目に入る中で最大になる拡散反射率
    float Fd90 = energyBias + 2.0f * dotLH * dotLH * roughness;
    //物体の法線と光への内積
    float dotNL = saturate(dot(N, L));
    //法線の向きによって適応する反射率を決定
    float FL = Fd90 + (dotNL - Fd90);
    //法線の向きとカメラへのベクトルの内積
    float dotNV = saturate(dot(N, V));
    //法線とカメラへのベクトルの近似具材を利用して適応する反射率を決定
    float FV = Fd90 + (dotNV - Fd90);
    //それぞれのベクトルの近似具合を利用して最終的に適用する反射率を決定
    //円周率で正規化
    return (FL * FV) / PI;
}

float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric)
{
    float microfacet = 0.76f;

	// 金属度を垂直入射の時のフレネル反射率として扱う
	// 金属度が高いほどフレネル反射は大きくなる
    float f0 = metaric;

	// ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める
    float3 H = normalize(L + V);

	// 各種ベクトルがどれくらい似ているかを内積を利用して求める
    float NdotH = saturate(dot(N, H));
    float VdotH = saturate(dot(V, H));
    float NdotL = saturate(dot(N, L));
    float NdotV = saturate(dot(N, V));

	// D項をベックマン分布を用いて計算する
    float D = Beckmann(microfacet, NdotH);

	// F項をSchlick近似を用いて計算する
    float F = SpcFresnel(f0, VdotH);

	// G項を求める
    float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));

	// m項を求める
    float m = PI * NdotV * NdotH;

	// ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める
    return max(F * D * G / m, 0.0);
}


/****************ここまでPBR関数*********************************************************************/


float4 PSMain( PSInput In ) : SV_Target0
{   
     //色をとってくる
    float4 albedoColor = albedoTexture.Sample(Sampler, In.uv);

    //法線を取得
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    float4 worldPos;

    // まず正規化スクリーン座標系での座標を計算する。
    // z座標は深度テクスチャから引っ張ってくる。
    worldPos.z = specDepthTexture.Sample(Sampler, In.uv).a;
    // xy座標はUV座標から計算する。
    worldPos.xy = In.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
    worldPos.w = 1.0f;
   
    // ビュープロジェクション行列の逆行列を乗算して、ワールド座標に戻す。
    worldPos = mul(viewProjInverseMatrix, worldPos);
    worldPos.xyz /= worldPos.w;
    worldPos.w = 1.0f;
    
    //物体から目へのベクトル
    float3 toEye = abs(eyePos - worldPos.xyz);
    toEye = normalize(toEye);
    
	///*スポットライト計算開始-------------------------------------------------------------------------------*/
	//最終的なスポットライト
    float4 finalSpotLight = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < SpotLightNum; i++)
    {
		//ポイントライトから物体へのベクトル
        float3 pointLightToSurface = worldPos.xyz - spotLight[i].spotPosition;
        
		//ポイントライトから物体への距離
        float3 distance = length(pointLightToSurface);
        pointLightToSurface = normalize(pointLightToSurface);
	
	//距離による影響度
        float distanceAffect = 1.0f - 1.0f / spotLight[i].spotRange * distance;
        distanceAffect = max(0.0f, distanceAffect);
        distanceAffect = pow(distanceAffect, 1.0f);

	//角度による影響度
        float angle = dot(pointLightToSurface, spotLight[i].spotDirection);
        angle = abs(acos(angle));
        float angleAffect = 1.0f - 1.0f / spotLight[i].spotAngle * angle;
        angleAffect = max(0.0f, angleAffect);
        angleAffect = pow(angleAffect, 2.0f);

	//ポイントライト拡散反射光
        float t = dot(pointLightToSurface, normal);
        t *= -1.0f;
        t = max(0.0f, t);
        float3 pointDiff = spotLight[i].spotColor * t;

	//ポイントライト鏡面反射光
        float3 pointRef = reflect(pointLightToSurface, normal);
        t = dot(toEye, pointRef);
        t = max(0.0f, t);
        t = pow(t, 2.0f);
        float3 pointSpec = spotLight[i].spotColor * t;

	//スポットライトにする
        pointDiff *= distanceAffect;
        pointSpec *= distanceAffect;

        float3 spotDiff = pointDiff * angleAffect;
        float3 spotSpec = pointSpec * angleAffect;
		
        finalSpotLight.xyz += spotDiff + spotSpec;
    }
    	/*スポットライト計算終わり-------------------------------------------------------------------------------*/
    finalSpotLight.w = 1.0f;
    //return finalSpotLight;
    
    /*　PBR計算開始　**********************************************************************************************/
    
    float3 lig = 0.0f;
	////拡散光の影響度を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight.direction, toEye);
	//物体の法線向きとディレクションライトの向きがどれだけ似ているか計算
    float NdotL = saturate(dot(normal, -directionalLight.direction));
	//正規化ランバート拡散反射光
    float3 lambertDiffuse = (directionalLight.color * NdotL) / PI;
	//最終的に適用する拡散反射光を計算
    float3 diffuse = albedoColor.xyz * diffuseFromFresnel * lambertDiffuse;
    
    //float4 a = 1.0f;
    //a.xyz = diffuseFromFresnel;
    //return a;
    
    //反射の具合を取得
    float specPower = normalTexture.Sample(Sampler, In.uv).w;
    //反射光の取得
    float3 specularColor = specDepthTexture.SampleLevel(Sampler, In.uv, 0).rgb;
    //クックトランスモデルを利用した鏡面反射率を計算する
    float3 spec = CookTrranceSpecular(-directionalLight.direction, toEye, normal, specPower) * directionalLight.color;

    
	//金属度（metaric)が強ければ、色は鏡面反射のspecularColor、弱ければ白。
	//SpecularColorの強さを鏡面反射の強さとして扱う。
    float specTerm = saturate(length(specularColor.xyz));
	//ここで金属度specPowerを利用して、白っぽい色から物体の色へ線形補完する。
    spec *= lerp(float3(specTerm, specTerm, specTerm), specularColor, specPower);

	//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する
    lig += diffuse * (1.0f - specTerm) + spec;
	
	//AOマップに似せたもの
    float3 ambient = ambientLight;
    //ディレクションライトと法線の内積
    float ambientPower = dot(-directionalLight.direction, normal);
    //最低でも少しは環境光適用のため
    ambientPower = max(0.6f, ambientPower);
    lig += ambient * ambientPower;
    
    float4 finalColor = albedoColor;
    finalColor.xyz *= lig;
    //モデルに適用したテクスチャの透過率が適用
    finalColor.a = albedoColor.a;
   
	/////* PBR完成 **********************************************************************************************/
    
    /*NoPBR*************************************************************************************************/
    //ピクセルの法線とライトの方向の内積を計算する。
 //   float t = dot(normal, -directionalLight.direction);
	////内積の結果が0以下なら0にする。
 //   t = max(0.0f, t);
	////拡散反射光を求める。
 //   float3 diffuseLig = directionalLight.color * t;
	
 //   //diffuseLig /= 3.14f;
 //   //float4 a;
 //   //a.xyz = diffuseLig * albedoColor.xyz;
 //   //return a;
    
	////step-4 反射ベクトルを求める。
 //   float3 refVec = reflect(directionalLight.direction, normal);
	////step-5 光が当たったサーフェイスから視点に伸びるベクトルを求める。
 //   //float3 toEye = eyePos - worldPos;
 //  //toEye = normalize(toEye);
	////step-6 鏡面反射の強さを求める。
 //   t = dot(refVec, toEye);
 //   if (t < 0.0f)
 //   {
 //       t = 0.0f;
 //   }
	////step-7 鏡面反射の強さを絞る。
 //   t = pow(t, 1.0f);

	////step-8 鏡面反射光を求める。
 //   float3 specularLig = directionalLight.color * t;

	////step-9 拡散反射光と鏡面反射光を足し算して、最終的な光を求める。
 //   float3 lig = diffuseLig + specularLig;

 //   float4 finalColor = albedoTexture.Sample(Sampler, In.uv);
	////step-10 テクスチャカラーに求めた光を乗算して最終出力カラーを求める。
 //   finalColor.xyz *= lig;
   // finalColor.xyz += ambientLight;
    //return finalColor;
    
    
    
    
    
    
    //ここからデプスシャドウの作成///////////////////////////////////////////////////////////////////////////
	//ライトビュースクリーン空間からUV空間に座標変換。
    
    float4 lvp = mul(LVP, worldPos);
    
    float2 shadowMapUV = lvp.xy / lvp.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
   
	//ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = lvp.z / lvp.w;

    if (shadowMapUV.x > 0.0f
		&& shadowMapUV.x < 1.0f
		&& shadowMapUV.y > 0.0f
		&& shadowMapUV.y < 1.0f)
    {
		//シャドウマップに描き込まれているZ値と比較する
        float zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV).r;
		//シャドウアクネ解決のため実数値で補正、調整
        if (zInLVP > zInShadowMap + 0.00007f)
        {
            finalColor.xyz *= 0.5f;
        }
    }

    float4 returnColor = finalColor + finalSpotLight;
    return returnColor;
	//ここまでデプスシャドウマップ///////////////////////////////////////////////////////////////////////////
}