/*!
 * @brief	影が落とされるモデル用のシェーダー。
 */

static const float PI = 3.14f;
static const int SpotLightNum = 2;

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

/*PBR用のライトデータ*/
struct DirectionalLight {
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
cbuffer ShadowCb : register(b1) {
	float4x4 mLVP;

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

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 			: POSITION;		//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	/*********/
	float3 tangent		: TANGENT;
	float3 biNormal		: BINORMAL;
	//SSkinVSIn skinVert;
	/*********/
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	/*今は不使用**********************/
	float3 tangent      : TANGENT;		//タンジェント
	float3 biNormal     : BINORMAL;		//
	/****************/
	//step-4 ライトビュースクリーン空間での座標を追加。
	float4 posInLVP : TEXCOORD1;
	float4 worldPos	: TEXCOORD2;		//ワールド座標
	
    float3 depthInView : TEXCOORD3;			//深度
};

//被写界深度用に深度値を持たせた構造体。
struct SPSOut
{
	float4 s_color : SV_Target0;
    float4 s_normal : SV_Target1;
	float  s_depth : SV_Target2;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);		//アルベドマップ。
Texture2D<float4> g_normalMap : register(t1);		//法線マップ。
Texture2D<float4> g_specMap : register(t2);		//鏡面マップ
Texture2D<float4> g_aoMap : register(t4);		//アンビエントオクルージョンマップ
Texture2D<float4> g_shadowMap : register(t10);	//シャドウマップ。

sampler g_sampler : register(s0);				// サンプラステート。

// 関数定義。
//スキン行列を計算する。
//float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
//{
//	float4x4 skinning = 0;
//	float w = 0.0f;
//	[unroll]
//	for (int i = 0; i < 3; i++)
//	{
//		skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
//		w += skinVert.Weights[i];
//	}
//
//	skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
//
//	return skinning;
//}

/// <summary>
/// 影が落とされる3Dモデル用の頂点シェーダー。スキン無し
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	//通常の座標変換。
	SPSIn psIn;
	//ワールド座標に変換
	psIn.pos = mul(mWorld, vsIn.pos);
	//ワールド座標を保存
	psIn.worldPos = psIn.pos;
	//ビュー座標に変換
	psIn.pos = mul(mView, psIn.pos);
	//深度値を取得（カメラからの深度値が欲しいため、このタイミング
    psIn.depthInView = psIn.pos.z;
	//スクリーン座標に変換
	psIn.pos = mul(mProj, psIn.pos);
	//法線情報の獲得
	psIn.normal = mul(mWorld, vsIn.normal);
	//正規化
	psIn.normal = normalize(psIn.normal);
	/*********************/
	//psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
	//psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
	/************************/
	//uv座標を保存
	psIn.uv = vsIn.uv;
	//ライトビュースクリーン空間の座標を計算する。
	psIn.posInLVP = mul(mLVP, psIn.worldPos);

	return psIn;
}

/****************ここからPBR関数*********************************************************************/

float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
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

/// <summary>
/// 影が落とされる3Dモデル用のピクセルシェーダー。
/// </summary>
SPSOut PSMain(SPSIn psIn) : SV_Target0
{
	//物体から目へのベクトル
	float3 toEye = eyePos - psIn.worldPos;
	toEye = normalize(toEye);
	/*スポットライト計算開始-------------------------------------------------------------------------------*/
	//最終的なスポットライト
    float4 finalSpotLight = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < SpotLightNum; i++)
    {
		//ポイントライトから物体へのベクトル
        float3 pointLightToSurface = psIn.worldPos - spotLight[i].spotPosition;
		//ポイントライトから物体への距離
        float3 distance = length(pointLightToSurface);
        pointLightToSurface = normalize(pointLightToSurface);
	
	//距離による影響度
        float distanceAffect = 1.0f - 1.0f / spotLight[i].spotRange * distance;
        if (distanceAffect < 0.0f)
        {
            distanceAffect = 0.0f;
        }
        distanceAffect = pow(distanceAffect, 1.0f);

	//角度による影響度
        float angle = dot(pointLightToSurface, spotLight[i].spotDirection);
        angle = abs(acos(angle));
        float angleAffect = 1.0f - 1.0f / spotLight[i].spotAngle * angle;
        if (angleAffect < 0.0f)
        {
            angleAffect = 0.0f;
        }
        angleAffect = pow(angleAffect, 0.5f);

	//ポイントライト拡散反射光
        float t = dot(pointLightToSurface, psIn.normal);
        t *= -1.0f;
        t = max(0.0f, t);
        float3 pointDiff = spotLight[i].spotColor * t;

	//ポイントライト鏡面反射光
        float3 pointRef = reflect(pointLightToSurface, psIn.normal);
        t = dot(toEye, pointRef);
        t = max(0.0f, t);
        t = pow(t, 5.0f);
        float3 pointSpec = spotLight[i].spotColor * t;

	//スポットライトにする
        pointDiff *= distanceAffect;
        pointSpec *= distanceAffect;

        float3 spotDiff = pointDiff * angleAffect;
        float3 spotSpec = pointSpec * angleAffect;
		
        finalSpotLight.xyz += pointDiff + pointSpec;
    }
	/*スポットライト計算終わり-------------------------------------------------------------------------------*/

	/*　PBR計算開始　**********************************************************************************************/
	float3 lig = 0;
	//float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);
	//todo tangentがないモデルがあったので、今回は法線マップはあきらめる。
	float3 normal = psIn.normal;

	//アルベドカラー、鏡面反射率の取得
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	
	float3 specColor = g_specMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
	float metaric = g_specMap.Sample(g_sampler, psIn.uv).a;
	metaric *= 1.2f;
	//目から物体のワールド座標への向き計算
	//float3 toEye = normalize(eyePos - psIn.worldPos);
	//拡散光の影響度を計算
	float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, directionalLight.direction, toEye);
	//物体の法線向きとディレクションライトの向きがどれだけ似ているか計算
	float NdotL = saturate(dot(normal, directionalLight.direction));
	//光の強さ計算
	float3 lambertDiffuse = directionalLight.color * NdotL / PI;
	//最終的に適用する拡散反射光を計算
	float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

	//下のようにHLSLではデバックすることもできる。
	//return (albedoColor * diffuseFromFresnel);

	//クックトランスモデルを利用した鏡面反射率を計算する
	float3 spec = CookTrranceSpecular(directionalLight.direction, toEye, normal, metaric) * directionalLight.color;

	//金属度（metaric)が強ければ、色は鏡面反射のspecularColor、弱ければ白。
	//SpecularColorの強さを鏡面反射の強さとして扱う。
	float specTerm = length(specColor.xyz);
	//ここで金属度metaricを利用して、白っぽい色から物体の色へ線形補完する。
	spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

	//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する
	lig += diffuse * (1.0f - specTerm) + spec;
	
	//AOマップを使用した環境光の適用
    float3 ambient = ambientLight;
    float ambientPower = g_aoMap.Sample(g_sampler, psIn.uv);
    ambient *= ambientPower;

	lig += ambient * albedoColor;

	float4 finalColor = 1.0f;
	finalColor.xyz = lig;
	//出力するカラーにアルベドマップのアルファ値を渡すとモデルに適用したテクスチャの透過率が適用される
	finalColor.a = albedoColor.a;
	//PBR完成
	//return finalColor;


	/*　PBR計算終わり　**********************************************************************************************/

	//ここからデプスシャドウの作成///////////////////////////////////////////////////////////////////////////
	//ライトビュースクリーン空間からUV空間に座標変換。
	float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;

	//ライトビュースクリーン空間でのZ値を計算する
	float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;

	if (shadowMapUV.x > 0.0f
		&& shadowMapUV.x < 1.0f
		&& shadowMapUV.y > 0.0f
		&& shadowMapUV.y < 1.0f) {
		//シャドウマップに描き込まれているZ値と比較する
		float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
		//シャドウアクネ解決のため実数値で補正、調整
		if (zInLVP > zInShadowMap + 0.00001f)
		{
			finalColor.xyz *= 0.5f;
		}
	}
	//return color;
    //return finalColor + finalSpotLight;
	//ここまでデプスシャドウマップ///////////////////////////////////////////////////////////////////////////
	
    SPSOut Out;
    Out.s_color = finalColor + finalSpotLight;
    Out.s_depth = psIn.depthInView;
    return Out;
}