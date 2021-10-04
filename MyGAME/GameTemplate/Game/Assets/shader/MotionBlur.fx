
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

static const float offSetX = 0.2f / 1280.0f;
static const float offSetY = 0.2f / 720.0f;

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
    
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, offSetY) * velocity.xy * (i + 1));
    }
    finalColor /= (float)loopCnt;

  /*      NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
        NearVelocityPower /= 9.0f;*/
    
    ////X方向及びY方向にだけブラーを掛ける////////////////////////////////////////////////////////////////////////
    //NearVelocityPower = normalize(NearVelocityPower); //lerp(-1, 1, NearVelocityPower.x);
    //NearVelocityPower.y = lerp(-1, 1, NearVelocityPower.y);
    
    //if (NearVelocityPower.x > 0.0f)
    //{
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * 2, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * 3, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * 4, 0)); //1つ右の中
    //    finalColor /= 5.0f;
    //}
    //else if (NearVelocityPower.x < 0.0f)
    //{
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * 2, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * 3, 0)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * 4, 0)); //1つ右の中
    //    finalColor /= 5.0f;
    //}
    
    //if (NearVelocityPower.y < 0.0f)
    //{

    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY * 2)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY * 3)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY * 4)); //1つ右の中
    //    finalColor /= 5.0f;
    //}
    //else if (NearVelocityPower.y > 0.0f)
    //{

    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY * 2)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY * 3)); //1つ右の中
    //    finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY * 4)); //1つ右の中
    //    finalColor /= 5.0f;
    //}
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

  /*  float blurPowerX = length(NearVelocityPower.x);
    blurPowerX = clamp(blurPowerX, 0.0f, 1.5f);
    float blurPowerY = length(NearVelocityPower.y);
    blurPowerY = clamp(blurPowerY, 0.0f, 1.5f);
  */   
    
    
    //if (NearVelocityPower.x != 0.0f || NearVelocityPower.y != 0.0f)
    //{
   /*     finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * blurPowerX, offSetY * blurPowerY)); //左上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * blurPowerX, 0)); //左
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX * blurPowerX, -offSetY * blurPowerY)); //左下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY * blurPowerY)); //上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY * blurPowerY)); //下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * blurPowerX, offSetY * blurPowerY)); //右上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * blurPowerX, 0)); //右
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX * blurPowerX, -offSetY * blurPowerY)); //右下
        finalColor /= 9.0f;*/
    //}
    
    //finalColor.xyz = pow(max(finalColor.xyz, 0.001f), 2.2f);
   // finalColor.xyz = pow(max(finalColor.xyz, 0.001f), 1.8f /*2.2f*/);
    
    finalColor.w = 1.0f;
    return finalColor;
}