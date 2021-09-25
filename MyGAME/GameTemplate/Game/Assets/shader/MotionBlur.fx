/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

static const float PI = 3.14f;
static const int SpotLightNum = 2;

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

Texture2D<float4> albedoMap : register(t0);
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
    float4 sceneMap = albedoMap.Sample(Sampler, In.uv);
    float4 normal = normalMap.Sample(Sampler, In.uv);
    float4 spec = specMap.Sample(Sampler, In.uv);
    float4 localVelocityMap = velocityMap.Sample(Sampler, In.uv);

    //if (length(localVelocityMap.r) != 0.0f)
    //{
    //    return normal;
    //}
    //else if (length(localVelocityMap.g) != 0.0f)
    //{
    //    return normal;
    //}
    //else if (length(localVelocityMap.b) != 0.0f)
    //{
    //    return sceneMap;
    //}
    float4 finalColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    float blurPowerX = length(localVelocityMap.x);
    float blurPowerY = length(localVelocityMap.y);
    blurPowerX = lerp(0, 1, blurPowerX);
    blurPowerY = lerp(0, 1, blurPowerY);
    
    if (blurPowerX > 0.0f || blurPowerY > 0.0f)
    {
        return finalColor;
    }
    
    float offSetX = 4.0f/1280.0f;
    float offSetY = 4.0f / 720.0f;
    
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
        float4 NearVelocityPower = 0.0f;
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
        //速度の減衰
        //NearVelocityPower /= 2.0f;
        NearVelocityPower.x = length(NearVelocityPower.x);
        NearVelocityPower.y = length(NearVelocityPower.y);
        NearVelocityPower.x = lerp(0, 1, NearVelocityPower.x);
        NearVelocityPower.y = lerp(0, 1, NearVelocityPower.y);
        
        if (length(NearVelocityPower.x) > 0.0f)
        {
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
            finalColor /= 9.0f;
        }
        else if (length(NearVelocityPower.y) > 0.0f)
        {

            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, offSetY)); //左上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, 0)); //左
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(-offSetX, -offSetY)); //左下
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(0, offSetY)); //上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(0, -offSetY)); //下
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, offSetY)); //右上
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, 0)); //右
            finalColor += albedoMap.Sample(Sampler, In.uv + float2(offSetX, -offSetY)); //右下
            finalColor /= 9.0f;
        }
    //}
    
    return finalColor;
    
   // return spec;
}