
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

float4 PSMain( PSInput In ) : SV_Target0
{       
    float4 finalColor = sceneMap.Sample(Sampler, In.uv);
    float4 localVelocityMap = velocityMap.Sample(Sampler, In.uv);
    float depth = specMap.Sample(Sampler, In.uv).w;

    //float4 finalColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    
    //もともとのオブジェクトにブラーを掛けないようにするため
    //float blurPowerX = length(localVelocityMap.x);
    //float blurPowerY = length(localVelocityMap.y);
    //blurPowerX = lerp(0.0f, 1.0f, blurPowerX);
    //blurPowerY = lerp(0.0f, 1.0f, blurPowerY);
    
    //if (blurPowerX > 0.0f || blurPowerY > 0.0f)
    //{
    //    return finalColor;
    //}
    //ここまで
    

    
    //ここからはスピードを持っているであろうピクセルのブラー
    //X方向に移動している時、X方向のみにブラー
    //if (length(localVelocityMap.x) != 0.0f)
    //{
    //    for (int i = 0; i < 10; i++)
    //    {
      
    //        finalColor += albedoMap.Sample(Sampler, In.uv + (blurPowerX * float2(i, 0)));
    //    }
    //    finalColor /= 10.0f;
    //}
    
    ////Y方向のみにブラー
    //if (length(localVelocityMap.y) != 0.0f)
    //{
    //    for (int i = 0; i < 10; i++)
    //    {
      
    //        finalColor += albedoMap.Sample(Sampler, In.uv + (blurPowerY * float2(0, i)));
    //    }
    //    finalColor /= 10.0f;
    //}
    //ここまで
    
    //速度がないピクセルの時
    //if (length(localVelocityMap.x) <= 0.1f)
    //{
    
    float offSetX = 6.0f / 1280.0f;
    float offSetY = 6.0f / 720.0f;

    
    
    float4 NearVelocityPower = velocityMap.Sample(Sampler, In.uv);
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
        NearVelocityPower += velocityMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
        //八方の速度の平均
        NearVelocityPower /= 9.0f;
    
    //XorY方向にだけブラーを掛ける/////////////////////////////////////////////////////////////////////////////

    ////X座標に関する速度を求める

    //if (NearVelocityPower.x < 0.0f)
    //{
    //    NearVelocityPower.x = -length(NearVelocityPower.x);
    //}
    //else
    //{
    //    NearVelocityPower.x = length(NearVelocityPower.x);
    //}
        
    ////Y座標に関する速度を求める
    //if (NearVelocityPower.y < 0.0f)
    //{
    //    NearVelocityPower.y = -length(NearVelocityPower.y);
    //}
    //else
    //{
    //    NearVelocityPower.y = length(NearVelocityPower.y);
    //}
    

   
    
    ////X方向及びY方向にだけブラーを掛ける////////////////////////////////////////////////////////////////////////
    //NearVelocityPower = normalize(NearVelocityPower); //lerp(-1, 1, NearVelocityPower.x);
    ////NearVelocityPower.y = lerp(-1, 1, NearVelocityPower.y);
    

    
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
    
  
        
    if (NearVelocityPower.x != 0.0f)
    {
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
        finalColor /= 9.0f;
    }
    else if (NearVelocityPower.y != 0.0f)
    {
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
        finalColor += sceneMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
        finalColor /= 9.0f;
    }
    
    //finalColor.xyz = pow(max(finalColor.xyz, 0.001f), 1.0f / 1.4f /*2.2f*/);
    
    finalColor.w = 1.0f;
    return finalColor;
}