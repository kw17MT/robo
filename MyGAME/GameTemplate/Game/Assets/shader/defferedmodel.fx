/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const float PI = 3.1415926f;

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

struct DirectionalLight {
	float3 direction;
	float3 color;
};

struct SpotLight
{
	float3 spotPosition;
	float3 spotColor;
	float3 spotDirection;
	float spotRange;
};

cbuffer LightCb : register (b1) {
    float4x4 mLVP;
	
	DirectionalLight directionalLight;

	float3 eyePos;
	float specPow;
	float3 ambientLight;
	
	SpotLight spotLight0;
	SpotLight spotLight1;
};
	

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;	
	float3 normal	: NORMAL;	//モデルの頂点座標。
	float3 tangent  : TANGENT;
	float3 biNormal : BINORMAL;
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;
	float3 normal		: NORMAL;//スクリーン空間でのピクセルの座標。
	float3 tangent      : TANGENT;
	float3 biNormal     : BINORMAL;
	float2 uv 			: TEXCOORD0;	//uv座標。
	float4 worldPos		: TEXCOORD1;
    float4 posInLVP : TEXCOORD2;
};

struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float4 normal : SV_Target1; // 法線

    // step-6 ピクセルシェーダーからの出力にワールド座標を追加
    float4 worldPos : SV_Target2;
    float4 depth : SV_TARGET3;
};

// グローバル変数。
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specMap : register(t2);
Texture2D<float4> g_shadowMap : register(t10);

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

// 関数定義。
//スキン行列を計算する。
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

//頂点シェーダーのコア関数。
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else{
		m = mWorld;
	}

	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	psIn.normal = mul(m, vsIn.normal);
	psIn.normal = normalize(psIn.normal);
	
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	psIn.uv = vsIn.uv;
	
    psIn.posInLVP = mul(mLVP, psIn.worldPos);

	return psIn;
}

//スキンなしメッシュ用の頂点シェーダーのエントリー関数。
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}

//スキンありメッシュの頂点シェーダーのエントリー関数。
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}

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
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH , 2 * NdotH * NdotL / VdotH));

	// m項を求める
	float m = PI * NdotV * NdotH;

	// ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める
	return max(F * D * G / m, 0.0);
}



SPSOut PSMain(SPSIn psIn)
{
    // G-Bufferに出力
    SPSOut psOut;

    // アルベドカラーを出力
    psOut.albedo = g_albedo.Sample(g_sampler, psIn.uv);
	
    float3 localNormal = g_normalMap.Sample(g_sampler, psIn.uv).xyz;
    localNormal = (localNormal - 0.5f) * 2.0f;
    float3 normal = psIn.tangent * localNormal.x + psIn.biNormal * localNormal.y + psIn.normal * localNormal.z;
    // 法線を出力
    // 出力は0～1に丸められてしまうのでマイナスの値が失われてしまう
    // そのため-1～1を0～1に変換する
    // (-1 ～ 1) ÷ 2.0       = (-0.5 ～ 0.5)
    // (-0.5 ～ 0.5) + 0.5  = ( 0.0 ～ 1.0)
    psOut.normal.xyz = (normal / 2.0f) + 0.5f;
    psOut.normal.w = g_specMap.Sample(g_sampler, psIn.uv).r;
	
	//深度値を記録
    psOut.depth.x = psIn.pos.z;
	
	//ワールド座標を記録
    psOut.worldPos.xyz = psIn.worldPos;
    psOut.worldPos.w = 1.0f;
	
	/**シャドウマップ**********************************************************/
    float4 color = g_albedo.Sample(g_sampler, psIn.uv);

	//ライトビュースクリーン空間からUV空間に座標変換。
    float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

	//ライトビュースクリーン空間でのZ値を計算する
    float zInLVP = psIn.posInLVP.z / psIn.posInLVP.w;

    if (shadowMapUV.x > 0.0f
		&& shadowMapUV.x < 1.0f
		&& shadowMapUV.y > 0.0f
		&& shadowMapUV.y < 1.0f)
    {
		//シャドウマップに描き込まれているZ値と比較する
        float zInShadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV).r;
		//シャドウアクネ解決のため実数値で補正、調整
        if (zInLVP > zInShadowMap + 0.00001f)
        {
			//color.xyz *= 0.5f;
            color.xyz *= 0.5f;
        }
    }
    
    psOut.albedo += color;
    return psOut;
}