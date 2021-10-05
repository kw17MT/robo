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

static const float ghostNum = 9.0f;

float4 PSMain( PSInput In ) : SV_Target0
{
    float4 outColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 center = { 0.5f, 0.5f };

    //センターまでの線分
    float2 toCenter = center - In.uv.xy;
    //太陽の方向に戻る線分の長さ
    float2 fetchNearLight = toCenter / 3.0f;
    //センターと点対照のピクセルの座標
    float2 texturePos = In.uv + toCenter * 2.0f;
    float4 finalColor = sceneTexture.Sample(Sampler, texturePos);
    //N回分もどるor進んで太陽の色を取得しに行く
    for (int i = 0; i < ghostNum; i++)
    {
        finalColor += sceneTexture.Sample(Sampler, In.uv - (fetchNearLight * i));
        finalColor += sceneTexture.Sample(Sampler, In.uv + (fetchNearLight * i));
    }
    //ゴーストの色を調整
    finalColor /= ghostNum * 2.0f;
    
    //ゴーストがあるピクセルにセンターからの位置で色合いを変える
    if (length(finalColor.xyz))
    {
        finalColor.xy += toCenter / 4.0f;
        finalColor.xy /= 2.0f;
    }
    //透明度を与える。
    finalColor.w = 0.6f;
    return finalColor;
}