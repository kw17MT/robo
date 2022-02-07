//モーションブラー用シェーダー

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

Texture2D<float4> sceneMap : register(t0);              //ブラー済みゲームシーンのテクスチャ
Texture2D<float4> velocityMap : register(t1);           //ベロシティマップ

sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
    psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain( PSInput In ) : SV_Target0
{  
    const int NUM_WEIGHT = 16;
    float weights[NUM_WEIGHT];
    float total = 0;
    for (int i = 0; i < NUM_WEIGHT; i++)
    {
        weights[i] = exp(-0.5f * (float) (i * i) / 30.0f);
        total += weights[i];
    }
	// 規格化
    for (int i = 0; i < NUM_WEIGHT; i++)
    {
        weights[i] /= total;
    }
    
    //ピクセルの速度を取得
    float4 velocity = velocityMap.Sample(Sampler, In.uv);
    float4 finalColor = 0.0f;
    
    float t = 0.1f;

    for (int i = 0; i < NUM_WEIGHT; i++)
    {
        finalColor += sceneMap.Sample(Sampler, In.uv + (velocity.xy) * t * i) * weights[i];
    }
    finalColor.w = 1.0f;
    return finalColor;
}