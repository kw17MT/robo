/*!
 * @brief	シンプルなモデルシェーダー。
 */

cbuffer cb : register(b0)
{
	float4x4 mvp;       // MVP行列
	float4 mulColor;    // 乗算カラー
};

cbuffer cb1 : register(b1)
{
    float4x4 viewProjInverseMatrix;
}

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

Texture2D<float4> bokeTexture : register(t0);   //メイン画面のボケテクスチャ
Texture2D<float4> depthTexture : register(t1);  //深度値を持ったレンダリングターゲット

sampler Sampler : register(s0);

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

const int depthInViewStart = 800;
const int depthInViewEnd = 2000;

/*!
 * @brief 被写界深度のピクセルシェーダー
 */
float4 PSMain(PSInput psIn) : SV_Target0
{
    float4 depth = depthTexture.Sample(Sampler, psIn.uv);
    //サンプリングの場所を変えること
    float4 boke = bokeTexture.Sample(Sampler, psIn.uv);

    float4 worldPos = (0.0f,0.0f,0.0f,0.0f);

    // まず正規化スクリーン座標系での座標を計算する。
    // z座標は深度テクスチャから引っ張ってくる。
    worldPos.z = depth.w;
    // xy座標はUV座標から計算する。
    worldPos.xy = psIn.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
    worldPos.w = 1.0f;
   
    // ビュープロジェクション行列の逆行列を乗算して、ワールド座標に戻す。
    worldPos = mul(viewProjInverseMatrix, worldPos);
    worldPos.xyz /= worldPos.w;
    worldPos.w = 1.0f;
	
	//深度値800以下のものはピクセルキル
	//800以下は被写界深度しない
    clip(worldPos.w - 2000.0f);
	

	
	//2000でボケ度最高値に
    boke.a = min(1.0f, (worldPos.w - 2000) / 10);
	
    return boke;
}