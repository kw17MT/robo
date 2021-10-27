#include "stdafx.h"
#include "LensGhost.h"

void LensGhost::Init()
{
	m_ghostTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	lensGhostData.m_width = 1280;
	lensGhostData.m_height = 720;
	lensGhostData.m_fxFilePath = "Assets/shader/LensGhost.fx";
	lensGhostData.m_textures[0] = &m_ghostTarget.GetRenderTargetTexture();
	lensGhostData.m_alphaBlendMode = AlphaBlendMode_Add;
	lensGhostData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_lensGhost.Init(lensGhostData);
}

void LensGhost::Render(RenderContext& rc, RenderTarget& target)
{
	//レンズゴーストを書くターゲットに設定
	rc.WaitUntilToPossibleSetRenderTarget(m_ghostTarget);
	rc.SetRenderTargetAndViewport(m_ghostTarget);
	rc.ClearRenderTargetView(m_ghostTarget);
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::lensGhost);
	//太陽を描画
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_ghostTarget);
	//通常描画モードに直す
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	//メインとなるターゲットにレンズゴーストを上書きする。
	rc.WaitUntilToPossibleSetRenderTarget(target);
	rc.SetRenderTargetAndViewport(target);
	m_lensGhost.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(target);
}