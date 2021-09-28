/*!
 * @brief	シンプルなモデルシェーダー。
 */

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

cbuffer CalcVelocityMapMatrix : register(b1)
{
    float4x4 prevViewProjMatrix;
    float4x4 currentViewProjMatrix;
}

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
    float4 velocity : TEXCOORD3;
};

struct SPSOut
{
    float4 albedo : SV_Target0; // アルベド
    float4 normal : SV_Target1; // 法線
    float4 SpecAndDepth : SV_TARGET2;
    float4 velocity : SV_TARGET3;				//4枚目のレンダーターゲットに速度マップを書き込みする
};

// グローバル変数。
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);
Texture2D<float4> g_specMap : register(t2);
Texture2D<float4> g_velocityMap : register(t4);

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
	
   // psIn.posInLVP = mul(mLVP, psIn.worldPos);

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

SPSOut PSMain(SPSIn psIn)
{
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
	//鏡面反射の強さを取得
    psOut.normal.w = g_specMap.Sample(g_sampler, psIn.uv).r;
	
    psOut.SpecAndDepth.xyz = g_specMap.Sample(g_sampler, psIn.uv).rgb;
	//深度値を記録
    psOut.SpecAndDepth.w = psIn.pos.z;
	
	/* ベロシティマップ */
    //float4 prevVelocity = mul(float4(psIn.worldPos.xyz, 1.0f), prevViewProjMatrix);
    //float4 currentVelocity = mul(float4(psIn.worldPos.xyz, 1.0f), currentViewProjMatrix);
    
    //prevVelocity *= 6;
    //currentVelocity *= 6;
    
    //float a = 1.0f;
    //psOut.velocity.xy = currentVelocity.xy / a - prevVelocity.xy / a;
    ////psOut.velocity *= 0.5f;
    ////psOut.velocity += 0.5f;
    //psOut.velocity.zw = 0.0f;
    //return psOut;
	
	/* ベロシティマップ zをx座標として利用 */
    float4 prevVelocity = mul(psIn.worldPos.xyzx, prevViewProjMatrix);
    float4 currentVelocity = mul(psIn.worldPos.xyzx, currentViewProjMatrix);
    
   // prevVelocity *= 6;
    //currentVelocity *= 6;
    
    float t = 1.0f;
    //psOut.velocity.xy = currentVelocity.wy / a - prevVelocity.wy / a;
    psOut.velocity.xy = currentVelocity.xy / t - prevVelocity.xy / t;
    //psOut.velocity *= 0.5f;
    //psOut.velocity += 0.5f;
    psOut.velocity.zw = 0.0f;
    return psOut;
	
	
}