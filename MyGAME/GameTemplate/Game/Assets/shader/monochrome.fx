/*!
 * @brief モノクロ処理シェーダー
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

cbuffer cb2 : register(b1)
{
    float monochromeRate;
}

struct VSInput{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`���B
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
	return colorTexture.Sample(Sampler, In.uv) * mulColor;
}

float4 PSMonochrome(PSInput In) : SV_Target0
{
    float4 finalColor = colorTexture.Sample(Sampler, In.uv);
	
    float Y = (0.299f * finalColor.r * monochromeRate) 
		+ (0.587f * finalColor.b * monochromeRate)
		+ (0.114f * finalColor.b * monochromeRate);
	
    finalColor.r = Y;
    finalColor.g = Y;
    finalColor.b = Y;
	
    return finalColor;
}