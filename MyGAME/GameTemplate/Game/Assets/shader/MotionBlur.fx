
cbuffer cb : register(b0){
	float4x4 mvp;		
	float4 mulColor;	
};

//ライトビュープロジェクション行列にアクセスする定数バッファを定義。
cbuffer constantBuffer : register(b1)
{
    float4x4 currentViewProjMatrix;
    float4x4 prevViewProjectionMatrix;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> sceneMap : register(t0);
Texture2D<float4> normalMap : register(t1);
Texture2D<float4> specMap : register(t2);
Texture2D<float4> velocityMap : register(t3);

sampler Sampler : register(s0);

PSInput VSMain(VSInput In) 
{
	PSInput psIn;
    psIn.pos = mul( mvp, In.pos );
	psIn.uv = In.uv;
	return psIn;
}

static const float offSetX = 0.4f / 1280.0f;
static const float offSetY = 0.4f / 720.0f;

float4 PSMain( PSInput In ) : SV_Target0
{       
    float4 finalColor = sceneMap.Sample(Sampler, In.uv);
    float4 localVelocityMap = velocityMap.Sample(Sampler, In.uv);
    float depth = specMap.Sample(Sampler, In.uv).w;

    //ピクセルの速度を取得
    float4 velocity =  velocityMap.Sample(Sampler,In.uv);
    int loopCnt = 16;
    for (int i = 0; i < loopCnt; i++)
    {
    
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY) * velocity.xy * (i + 1));
    }
    finalColor /= (float)loopCnt;

    
    finalColor.w = 1.0f;
    return finalColor;
}