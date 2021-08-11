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

Texture2D<float4> bokeTexture : register(t0); // メインレンダリングターゲットのテクスチャ
Texture2D<float4> depthTexture : register(t1);

sampler Sampler : register(s0);

const int depthInViewStart = 800;
const int depthInViewEnd = 2000;


/*!
 * @brief 被写界深度のピクセルシェーダー
 */
float4 PSMain(PSInput psIn) : SV_Target0
{
    float depth = depthTexture.Sample(Sampler, psIn.uv);

	//深度値800以下のものはピクセルキル
	//800以下は被写界深度しない
    clip(depth - 1590.0f);
	
    float4 boke = bokeTexture.Sample(Sampler, psIn.uv);
	
	//2000でボケ度最高値に
    boke.a = min(1.0f, (depth - 1590) / 35);
	
    return boke;
}