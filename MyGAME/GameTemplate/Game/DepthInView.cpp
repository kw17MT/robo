#include "stdafx.h"
#include "DepthInView.h"

void DepthInView::Init(RenderTarget& rt)
{
	m_depthInViewMap.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	//ブルーム画像の初期化用
	SpriteInitData bloomSpriteData;
	bloomSpriteData.m_fxFilePath = "Assets/shader/blur/bloom.fx";
	bloomSpriteData.m_vsEntryPointFunc = "VSMain";
	bloomSpriteData.m_psEntryPoinFunc = "PSLuminance";
	bloomSpriteData.m_width = rt.GetWidth();
	bloomSpriteData.m_height = rt.GetHeight();
	//輝度を抽出したい場面を取得
	bloomSpriteData.m_textures[0] = &rt.GetRenderTargetTexture();
	bloomSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_main.Init(bloomSpriteData);

	SpriteInitData finalSpriteData;

	finalSpriteData.m_textures[0] = &m_depthGaussian.GetBokeTexture();
	finalSpriteData.m_textures[1] = &m_depthInViewMap.GetRenderTargetTexture();
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;
	finalSpriteData.m_fxFilePath = "Assets/shader/blur/depthInView.fx";
	finalSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Trans;
	m_finalSprite.Init(finalSpriteData);

	m_depthTargets[0] = &rt;
	m_depthTargets[1] = &m_depthInViewMap;

	m_depthGaussian.Init(&rt.GetRenderTargetTexture());
}

void DepthInView::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	rc.WaitUntilToPossibleSetRenderTargets(2, m_depthTargets);
	rc.SetRenderTargetsAndViewport(2, m_depthTargets);
	rc.ClearRenderTargetViews(2, m_depthTargets);
	m_main.Draw(rc);

	rc.WaitUntilFinishDrawingToRenderTargets(2, m_depthTargets);
	m_depthGaussian.ExecuteOnGPU(rc, 120);
}