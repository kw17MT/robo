/*!
 * @brief フォワードでのレンズゴースト用シェーダー
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
    PSInput psIn = (PSInput)0;
	psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

static const float ghostNum = 8.0f;
static const float maxUV = 1.0f;
static const float minUV = 0.0f;

float4 PSMain( PSInput In ) : SV_Target0
{
    float4 outColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float2 center = { 0.5f, 0.5f };

    //センターまでの線分
    float2 toCenter = center - In.uv.xy;
    //太陽の方向に戻る線分の長さ
    float2 fetchNearLight = toCenter / 3.0f;
   
    
    float4 finalColor = sceneTexture.Sample(Sampler, In.uv);
    //N回分もどるor進んで太陽の色を取得しに行く
    for (int i = 0; i < ghostNum; i++)
    {
        float2 AddedUV = In.uv + (fetchNearLight * i);
        //if (AddedUV.x >= maxUV || AddedUV.x <= minUV
        //|| AddedUV.y >= maxUV || AddedUV.y <= minUV)
        //{
        //    continue;
        //}
        
        float2 SubtractedUV = In.uv - (fetchNearLight * i);
        //if (SubtractedUV.x >= maxUV || SubtractedUV.x <= minUV
        //|| SubtractedUV.y >= maxUV || SubtractedUV.y <= minUV)
        //{
        //    continue;
        //}
        
        finalColor += sceneTexture.Sample(Sampler, AddedUV );
        finalColor += sceneTexture.Sample(Sampler, SubtractedUV);
    }
    //ゴーストの色を調整
    //finalColor /= ghostNum * 2.0f;
    
    //ゴーストがあるピクセルにセンターからの位置で色合いを変える
    //if (length(finalColor.xyz))
    //{
    //    finalColor.xy += toCenter / 4.0f;
    //    finalColor.xy /= 3.0f;
    //}
    //透明度を与える。
    finalColor.w = 0.6f;
    return finalColor;
}