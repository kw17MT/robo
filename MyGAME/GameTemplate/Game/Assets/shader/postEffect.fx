/*!
 * @brief	シンプルなモデルシェーダー。
 */

cbuffer cb : register(b0)
{
	float4x4 mvp;       // MVP行列
	float4 mulColor;    // 乗算カラー
};

struct VSInput
{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

/*!
 * @brief 頂点シェーダー
 */
PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(mvp, In.pos);
	psIn.uv = In.uv;
	return psIn;
}

//Texture2D<float4> RenderTargetTexture : register(t0);

Texture2D<float4> RenderTargetTexture01 : register(t0); // メインレンダリングターゲットのテクスチャ
Texture2D<float4> RenderTargetTexture02 : register(t1);
Texture2D<float4> RenderTargetTexture03 : register(t2);
Texture2D<float4> RenderTargetTexture04 : register(t3);

sampler Sampler : register(s0);

/////////////////////////////////////////////////////////
// 輝度抽出用
/////////////////////////////////////////////////////////
/*!
 * @brief 輝度抽出用のピクセルシェーダー
 */
float4 PSLuminance(PSInput In) : SV_Target0
{
	//輝度を抽出するピクセルシェーダーを実装（ブラー1回だけ）
	/*float4 color = RenderTargetTexture.Sample(Sampler, In.uv);
	float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
	clip(t - 1.0f);

	return color;*/

	//4回ガウシアンブラーを行う
	float4 combineColor = RenderTargetTexture01.Sample(Sampler, In.uv);
	combineColor += RenderTargetTexture02.Sample(Sampler, In.uv);
	combineColor += RenderTargetTexture03.Sample(Sampler, In.uv);
	combineColor += RenderTargetTexture04.Sample(Sampler, In.uv);

	combineColor /= 4.0f;
	combineColor.a = 1.0f;

	return combineColor;
}