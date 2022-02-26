/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

//ライトビュープロジェクション行列にアクセスする定数バッファを定義。
cbuffer ShadowCb : register(b1)
{
    float4x4 LVPC[3];
    float4x4 viewProjInverseMatrix;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> albedoTexture : register(t0);
Texture2D<float4> normalTexture : register(t1);
Texture2D<float4> specDepthTexture : register(t2);
Texture2D<float4> shadowMap1 : register(t3);
Texture2D<float4> shadowMap2 : register(t5);
Texture2D<float4> shadowMap3 : register(t6);

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
     //色をとってくる
    float4 albedoColor = albedoTexture.Sample(Sampler, In.uv);
    
    //法線を取得
    float3 normal = normalTexture.Sample(Sampler, In.uv).xyz;
    normal = (normal * 2.0f) - 1.0f;
    
    float4 worldPos;

    // まず正規化スクリーン座標系での座標を計算する。
    // z座標は深度テクスチャから引っ張ってくる。
    worldPos.z = specDepthTexture.Sample(Sampler, In.uv).a;
    // xy座標はUV座標から計算する。
    worldPos.xy = In.uv * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
    worldPos.w = 1.0f;
   
    // ビュープロジェクション行列の逆行列を乗算して、ワールド座標に戻す。
    worldPos = mul(viewProjInverseMatrix, worldPos);
    worldPos.xyz /= worldPos.w;
    worldPos.w = 1.0f;
    
   
    
    
    //ここからデプスシャドウの作成///////////////////////////////////////////////////////////////////////////
	//ライトビュースクリーン空間からUV空間に座標変換。
    
 //   float4 lvp = mul(LVP, worldPos);
    
 //   float2 shadowMapUV = lvp.xy / lvp.w;
 //   shadowMapUV *= float2(0.5f, -0.5f);
 //   shadowMapUV += 0.5f;
   
	////ライトビュースクリーン空間でのZ値を計算する
 //   float zInLVP = lvp.z / lvp.w;

 //   if (shadowMapUV.x > 0.0f
	//	&& shadowMapUV.x < 1.0f
	//	&& shadowMapUV.y > 0.0f
	//	&& shadowMapUV.y < 1.0f)
 //   {
	//	//シャドウマップに描き込まれているZ値と比較する
 //       float zInShadowMap = shadowMap.Sample(Sampler, shadowMapUV).r;
	//	//シャドウアクネ解決のため実数値で補正、調整
 //       if (zInLVP > zInShadowMap + 0.00007f)
 //       {
 //           finalColor.xyz *= 0.5f;
 //       }
 //   }

 //   float4 returnColor = finalColor + finalSpotLight;
   
 //   return returnColor;
	//ここまでデプスシャドウマップ///////////////////////////////////////////////////////////////////////////
    
    
    //カスケード
    float4 posInLVP[3];
    posInLVP[0] = mul(LVPC[0], worldPos);
    posInLVP[1] = mul(LVPC[1], worldPos);
    posInLVP[2] = mul(LVPC[2], worldPos);
    
    float4 color = albedoTexture.Sample(Sampler, In.uv);
    Texture2D<float4> shadowMaps[3];
    shadowMaps[0] = shadowMap1;
    shadowMaps[1] = shadowMap2;
    shadowMaps[2] = shadowMap3;

    // step-16 3枚のシャドウマップを使って、シャドウレシーバーに影を落とす
    for (int cascadeIndex = 0; cascadeIndex < 3; cascadeIndex++)
    {
        float zInLVP = posInLVP[cascadeIndex].z / posInLVP[cascadeIndex].w;
        
        if (zInLVP >= 0.0f && zInLVP <= 1.0f)
        {
            float2 shadowMapUV = posInLVP[cascadeIndex].xy / posInLVP[cascadeIndex].w;
            shadowMapUV *= float2(0.5f, -0.5f);
            shadowMapUV += 0.5f;
            
            if (shadowMapUV.x >= 0.0f && shadowMapUV.x <= 1.0f && shadowMapUV.y >= 0.0f && shadowMapUV.y <= 1.0f)
            {
                float2 shadowValue = shadowMaps[cascadeIndex].Sample(Sampler, shadowMapUV).xy;
                if (zInLVP >= shadowValue.r)
                {
                    color.xyz *= 0.5f;
                    break;
                }
            }
        }
    }

    return albedoColor;
}