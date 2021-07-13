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

	//ライトビュースクリーン空間での座標
	float4 posInLVP : TEXCOORD1;
    float3 depthInView : TEXCOORD2;
};

struct SPSOut
{
    float4 s_color : SV_Target0;
    float s_depth : SV_Target1;
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
	//ワールド座標に変換
	float4 worldPos = mul(mWorld, vsIn.pos);
	//ビュー座標に変換
	psIn.pos = mul(mView, worldPos);
	//カメラからの深度を記録
    psIn.depthInView = psIn.pos.z;
	//プロジェクション行列に変換
	psIn.pos = mul(mProj, psIn.pos);
	//UVの座標を記録
	psIn.uv = vsIn.uv;

	//ライトビュースクリーン空間の座標を計算する。
	psIn.posInLVP = mul(mLVP, worldPos);
	psIn.normal = mul(mWorld, vsIn.normal);

	return psIn;
}
/// <summary>
/// 影が落とされる3Dモデル用のピクセルシェーダー。
/// </summary>
/*float4*/SPSOut PSMain( SPSIn psIn ) : SV_Target0
{
	//以下から投影シャドウマップの作成。/////////////////////////////////////////////////////////////////////
	//float4 color = g_albedo.Sample(g_sampler, psIn.uv);
	//color.x *= 4.5f;
	//color.y *= 4.5f;
	//color.z *= 4.5f;

	////ライトビュースクリーン空間からUV空間に座標変換。
	//float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
	//shadowMapUV *= float2(0.5f, -0.5f);
	//shadowMapUV += 0.5f;

	////計算したUV座標を使って、シャドウマップから影情報をサンプリング
	//float3 shadowMap = 1.0f;

	//if (shadowMapUV.x > 0.0f 
	//	&& shadowMapUV.x < 1.0f 
	//	&& shadowMapUV.y > 0.0f 
	//	&& shadowMapUV.y < 1.0f) {
	//	shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
	//}

	////サンプリングした影情報をテクスチャカラーに乗算する。
	//color.xyz *= shadowMap;

	//return color;
	//ここまで投影シャドウマップ/////////////////////////////////////////////////////////////////////////////

	//ここからデプスシャドウの作成///////////////////////////////////////////////////////////////////////////
	float4 color = g_albedo.Sample(g_sampler, psIn.uv);
	color.x *= 4.5f;
	color.y *= 4.5f;
	color.z *= 4.5f;

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
		if (zInLVP > zInShadowMap)
		{
			color.xyz *= 0.5f;
		}
	}

	//return color;
	//ここまでデプスシャドウマップ///////////////////////////////////////////////////////////////////////////
	
    SPSOut Out;
    Out.s_color = color;
    Out.s_depth = psIn.depthInView;
    return Out;
}