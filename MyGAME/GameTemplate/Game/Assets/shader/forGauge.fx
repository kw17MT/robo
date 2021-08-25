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

cbuffer LightCb : register (b1) 
{
	float3 ambientLight;
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
};

// グローバル変数。
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ

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

//ピクセルシェーダーのエントリー関数。
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 color = 1.0f;
    color.xyz = ambientLight;
    color.w = 1.0f;
    
    return color;
}