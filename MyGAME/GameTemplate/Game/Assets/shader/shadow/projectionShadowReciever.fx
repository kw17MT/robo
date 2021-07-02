/*!
 * @brief	影が落とされるモデル用のシェーダー。
 */

//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
//step-3 ライトビュープロジェクション行列にアクセする定数バッファを定義。
cbuffer ShadowCb : register(b1) {
	float4x4 mLVP;
}


//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 			: POSITION;		//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。

	//step-4 ライトビュースクリーン空間での座標を追加。
	float4 posInLVP : TEXCOORD1;
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0);		//アルベドマップ。
Texture2D<float4> g_shadowMap : register(t10);	//シャドウマップ。
sampler g_sampler : register(s0);				// サンプラステート。


/// <summary>
/// 影が落とされる3Dモデル用の頂点シェーダー。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	//通常の座標変換。
	SPSIn psIn;
	float4 worldPos = mul(mWorld, vsIn.pos);
	psIn.pos = mul(mView, worldPos);
	psIn.pos = mul(mProj, psIn.pos);
	psIn.uv = vsIn.uv;

	//step-5 ライトビュースクリーン空間の座標を計算する。
	psIn.posInLVP = mul(mLVP, worldPos);
	psIn.normal = mul(mWorld, vsIn.normal);

	return psIn;
}
/// <summary>
/// 影が落とされる3Dモデル用のピクセルシェーダー。
/// </summary>
float4 PSMain( SPSIn psIn ) : SV_Target0
{
	//以下から投影シャドウマップの作成。/////////////////////////////////////////////////////////////////////
	float4 color = g_albedo.Sample(g_sampler, psIn.uv);
	color.x *= 1.2f;
	color.y *= 1.2f;
	color.z *= 1.2f;

	//ライトビュースクリーン空間からUV空間に座標変換。
	float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;

	//計算したUV座標を使って、シャドウマップから影情報をサンプリング
	float3 shadowMap = 1.0f;

	if (shadowMapUV.x > 0.0f 
		&& shadowMapUV.x < 1.0f 
		&& shadowMapUV.y > 0.0f 
		&& shadowMapUV.y < 1.0f) {
		shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
	}

	//サンプリングした影情報をテクスチャカラーに乗算する。
	color.xyz *= shadowMap;

	return color;
	//ここまで投影シャドウマップ/////////////////////////////////////////////////////////////////////////////
}