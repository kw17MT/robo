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
Texture2D<float4> worldPosTexture   : register(t2);
Texture2D<float4> aoMap : register(t4);
	
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

float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    float3 H = normalize(L + V);

    float roughness = 0.5f;
    float energyBias = lerp(0.0f, 0.5f, roughness);

    float dotLH = saturate(dot(L, H));

    float Fd90 = energyBias + 2.0 * dotLH * dotLH * roughness;

    float dotNL = saturate(dot(N, L));

    float FL = Fd90 + (dotNL - Fd90);

    float dotNV = saturate(dot(N, V));

    float FV = Fd90 + (dotNV - Fd90);

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
    float3 normal = normalTexture.Sample(Sampler, In.uv);
    
    normal = (normal * 2.0f) - 1.0f;

    //ワールド座標を取得
    float3 worldPos = worldPosTexture.Sample(Sampler, In.uv).xyz;

    //float4 a = { 1.0, 1.0f, 1.0f, 1.0f };
    //a.xyz = normal;
    //return a;
    
    //return albedoColor;
    
    
    
    //物体から目へのベクトル
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);
    
	/*スポットライト計算開始-------------------------------------------------------------------------------*/
	//最終的なスポットライト
    float4 finalSpotLight = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    for (int i = 0; i < SpotLightNum; i++)
    {
		//ポイントライトから物体へのベクトル
        float3 pointLightToSurface = worldPos - spotLight[i].spotPosition;
        
        //float4 a = { 1.0, 1.0f, 1.0f, 1.0f };
        //a.xyz = pointLightToSurface;
        //return a;
        
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
        angleAffect = pow(angleAffect, 0.5f);

	//ポイントライト拡散反射光
        float t = dot(pointLightToSurface, normal);
        t *= -1.0f;
        t = max(0.0f, t);
        float3 pointDiff = spotLight[i].spotColor * t;

	//ポイントライト鏡面反射光
        float3 pointRef = reflect(pointLightToSurface, normal);
        t = dot(toEye, pointRef);
        t = max(0.0f, t);
        t = pow(t, 5.0f);
        float3 pointSpec = spotLight[i].spotColor * t;

	//スポットライトにする
        pointDiff *= distanceAffect;
        pointSpec *= distanceAffect;

        float3 spotDiff = pointDiff * angleAffect;
        float3 spotSpec = pointSpec * angleAffect;
		
        finalSpotLight.xyz += spotDiff + pointDiff;
        //finalSpotLight.xyz += pointDiff + pointSpec;
    }
    	/*スポットライト計算終わり-------------------------------------------------------------------------------*/
    finalSpotLight.w = 1.0f;
    //return finalSpotLight;
    
    /*　PBR計算開始　**********************************************************************************************/
    float3 lig = 0;
    //反射の具合を取得
    float metaric = normalTexture.Sample(Sampler, In.uv).w;

	//拡散光の影響度を計算
    float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionalLight.direction, toEye);
	//物体の法線向きとディレクションライトの向きがどれだけ似ているか計算
    float NdotL = saturate(dot(normal, -directionalLight.direction));
	//光の強さ計算
    float3 lambertDiffuse = directionalLight.color * NdotL / PI;
	//最終的に適用する拡散反射光を計算
    float3 localAlbedoColor = albedoColor.xyz;
    float3 diffuse = localAlbedoColor * diffuseFromFresnel * lambertDiffuse;

    //float4 a = { 1.0, 1.0f, 1.0f, 1.0f };
    //a.xyz = lambertDiffuse.xyz;
    //return a;
    
	//下のようにHLSLではデバックすることもできる。
	//return (albedoColor * diffuseFromFresnel);

	//クックトランスモデルを利用した鏡面反射率を計算する
    float3 spec = CookTrranceSpecular(directionalLight.direction, toEye, normal, metaric) * directionalLight.color;

	//金属度（metaric)が強ければ、色は鏡面反射のspecularColor、弱ければ白。
	//SpecularColorの強さを鏡面反射の強さとして扱う。
    
    /*ここのレングスの中身を正しいものにする*/
    float specTerm = length(normal.xyz/*specColor.xyz*/);
    
    
	//ここで金属度metaricを利用して、白っぽい色から物体の色へ線形補完する。
    spec *= lerp(float3(specTerm, specTerm, specTerm), normal.xyz/*specColor*/, metaric);

	//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する
    lig += diffuse * (1.0f - specTerm) + spec;
	
	//AOマップを使用した環境光の適用
    float3 ambient = ambientLight;
    float ambientPower = aoMap.Sample(Sampler, In.uv);
    
    ambient *= ambientPower;

    lig += ambient * albedoColor.xyz;

    float4 finalColor = 1.0f;
    finalColor.xyz = lig;
    
	//出力するカラーにアルベドマップのアルファ値を渡すとモデルに適用したテクスチャの透過率が適用される
    finalColor.a = albedoColor.a;
	/* PBR完成 **********************************************************************************************/

	////return finalColor;
    
    //float4 a = { 1.0f, 1.0f, 1.0f, 1.0f };
    //a.xyz = ambient;
    //return a;
    
    //float4 a;
    //a.xyz = directionalLight.direction;
    //return a;
    
     // 拡散反射光を計算
    float3 light = 0.0f;
    float t = max(0.0f, dot(normal, directionalLight.direction) * -1.0f);
    light = directionalLight.color * t;

    // スペキュラ反射を計算
    float3 r = reflect(directionalLight.direction, normal);
    t = max(0.0f, dot(toEye, r));
    t = pow(t, 5.0f);

    // このサンプルでは、スペキュラの効果を分かりやすくするために、50倍している
    float3 specColor = directionalLight.color * t * 5.0f;

    // step-3 スペキュラ強度を法線テクスチャのw要素からサンプリングする
    float specPower = normalTexture.Sample(Sampler, In.uv).w;

    // step-4 スペキュラ強度をスペキュラライトに乗算する
    specColor *= specPower;
    
    //反射光にスペキュラカラーを足し算する
    lig += specColor;

    //環境光。このサンプルでは一律で0.7底上げをする
    lig += 0.7f;

    float4 finalColora = albedoColor;
    finalColora.xyz *= lig;
    return finalColora;
    
   
   
    
    //float4 a;
    //a.xyz = normal;
    //return a;

    //float3 lig = 0.0f;
    ////法線による光の影響度計算
    //float t = max(0.0f, dot(normal, directionalLight.direction) * -1.0f);
    ////拡散反射光に影響度を適用
    //lig = directionalLight.color * t;
    
    
    
    //float4 a;
    //a.xyz = worldPos;
    //return a;
    

    //反射率の計算
    //float3 r = reflect(directionalLight.direction, normal);
    //t = max(0.0f, dot(toEye, r));
    //t = pow(t, 5.0f);
    
    //float3 specColor = directionalLight.color * t * 50.0f;
    
    ////float4 a;
    ////a.xyz = specColor;
    ////return a;
    
   
    //specColor *= specPower;
    
    
    //lig += specColor;
    //lig += ambientLight;
    
    //float4 finalColor = albedoColor;
    //finalColor.xyz *= lig;
    
    return finalColor;
}