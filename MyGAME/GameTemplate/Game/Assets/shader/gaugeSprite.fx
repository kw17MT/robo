/*!
 * @brief	�X�v���C�g�p�̃V�F�[�_�[�B
 */

cbuffer cb : register(b0){
	float4x4 mvp;		//���[���h�r���[�v���W�F�N�V�����s��B
	float4 mulColor;	//��Z�J���[�B
};

cbuffer cb2 : register(b1)
{
    float barRate;
    float scrollAmount;
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
Texture2D<float4> additionalTexture : register(t10);

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
	//ゲージ本体の色
    float4 albedoColor = colorTexture.Sample(Sampler, In.uv) * mulColor;
	
	//色がないところはピクセルキル
    if (albedoColor.w == 0.0f)
    {
        clip(-1.0f);
    }
	
	//画面の収縮を右から左にしたいため、uv座標を左右反転
    float uvX = In.uv.x * -1.0f;
    uvX += 1.0f;
	//収縮結果がbarRateに沿った画像サイズになるように、ピクセルキル
    clip(uvX - barRate);
	
	//定数バッファに乗ってきたスクロール度合いをuvに適用
    In.uv.x += scrollAmount;
	//uv値が1以上になり、はみ出ていたら
	if(In.uv.x >= 1.0f)
    {
		//反対側になるであろう箇所に巻き戻す
        In.uv.x -= 1.0f;
    }
	//UVスクロールの対象の加算する画像テクスチャ
    float4 anotherTexture = additionalTexture.Sample(Sampler, In.uv) / 5.0f;
	//加算して返す
    albedoColor.xyz += anotherTexture.xyz;
	
	//HPの減り具合に応じて色を変化させる。
	if(barRate >= 0.7f)
    {
        albedoColor.x *= (1.0f + barRate);
        albedoColor.y *= 0.3f;
        albedoColor.z *= 0.6f;
    }

    return albedoColor;
}