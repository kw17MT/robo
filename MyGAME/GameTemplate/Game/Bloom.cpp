#include "stdafx.h"
#include "Bloom.h"

void Bloom::Init(RenderTarget& rt)
{
	m_luminanceTarget.Create(
		rt.GetWidth(),   // 解像度はメインレンダリングターゲットと同じ
		rt.GetHeight(),  // 解像度はメインレンダリングターゲットと同じ
		1,
		1,
		rt.GetColorBufferFormat(),
		DXGI_FORMAT_D32_FLOAT
	);

	//ブルーム画像の初期化用
	SpriteInitData bloomSpriteData;	
	bloomSpriteData.m_fxFilePath = "Assets/shader/postEffect.fx";
	bloomSpriteData.m_vsEntryPointFunc = "VSMain";
	bloomSpriteData.m_psEntryPoinFunc = "PSLuminance";
	bloomSpriteData.m_width = rt.GetWidth();
	bloomSpriteData.m_height = rt.GetHeight();
	//輝度を抽出したい場面を取得
	bloomSpriteData.m_textures[0] = &rt.GetRenderTargetTexture();
	bloomSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_bloomSprite.Init(bloomSpriteData);

	//ガウシアンブラーの初期化
	m_gaussianBlur[0].Init(&m_luminanceTarget.GetRenderTargetTexture());
	m_gaussianBlur[1].Init(&m_gaussianBlur[0].GetBokeTexture());
	m_gaussianBlur[2].Init(&m_gaussianBlur[1].GetBokeTexture());
	m_gaussianBlur[3].Init(&m_gaussianBlur[2].GetBokeTexture());

	SpriteInitData finalSpriteData;

	//最終表示用の画像の初期化
	finalSpriteData.m_textures[0] = &m_gaussianBlur[0].GetBokeTexture();
	finalSpriteData.m_textures[1] = &m_gaussianBlur[1].GetBokeTexture();
	finalSpriteData.m_textures[2] = &m_gaussianBlur[2].GetBokeTexture();
	finalSpriteData.m_textures[3] = &m_gaussianBlur[3].GetBokeTexture();
	finalSpriteData.m_width = rt.GetWidth();
	finalSpriteData.m_height = rt.GetHeight();

	finalSpriteData.m_fxFilePath = "Assets/shader/spriteHalfAlpha.fx";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_finalSprite.Init(finalSpriteData);
}

void Bloom::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	//輝度抽出用に画像を描画する。
	rc.WaitUntilToPossibleSetRenderTarget(m_luminanceTarget);
	rc.SetRenderTargetAndViewport(m_luminanceTarget);
	rc.ClearRenderTargetView(m_luminanceTarget);
	m_bloomSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_luminanceTarget);

	//輝度の高い部分を4回連続でブラーをかける
	m_gaussianBlur[0].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[1].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[2].ExecuteOnGPU(rc, 5);
	m_gaussianBlur[3].ExecuteOnGPU(rc, 5);

	//もとの画像は消さずに、ブルームを適用する。
	rc.WaitUntilToPossibleSetRenderTarget(TargetToApply);
	rc.SetRenderTargetAndViewport(TargetToApply);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(TargetToApply);
}