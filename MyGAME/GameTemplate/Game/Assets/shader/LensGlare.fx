/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		
	float4 mulColor;	
};
struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> sceneTexture : register(t0);	
sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

//static const int N = 6;
//static const float rad = 8.0f;
//static const float PI = 3.14f;

float BLUR_RADIUS = 8.0f;
float BLUR_TEX_W = 1280.0f;
float BLUR_TEX_H = 720.0f;

float4 PSMain( PSInput In ) : SV_Target0
{

    //ブラーステップの長さを求める
    float blurStepLen = BLUR_RADIUS / 4.0f;

    // step-12 左斜め下方向へのUVオフセットを計算する
    float2 uvOffset;
    uvOffset.x = 0.86602f / BLUR_TEX_W;

    uvOffset.y = -0.5f / BLUR_TEX_H;
    uvOffset *= blurStepLen;

    // step-13 左斜め下方向にカラーをサンプリングする
    float4 color = sceneTexture.Sample(
        Sampler, In.uv + uvOffset);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 2);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 3);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 4);

    // step-14 右斜め下方向へのUVオフセットを計算する
    uvOffset.x = -0.86602f / BLUR_TEX_W * blurStepLen;

    // step-15 右斜め下方向にカラーをサンプリングする
    color += sceneTexture.Sample(
        Sampler, In.uv);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 2);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 3);

    color += sceneTexture.Sample(
        Sampler, In.uv + uvOffset * 4);

    // step-16 平均化
    color /= 9.0f;
    color.yz = 0.0f;

    return color;
}