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
Texture2D<float4> speedMap : register(t3);


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
    float4 map = speedMap.Sample(Sampler, In.uv);
    return map;
    
   
}