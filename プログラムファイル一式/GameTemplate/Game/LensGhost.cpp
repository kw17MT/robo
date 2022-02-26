#include "stdafx.h"
#include "LensGhost.h"

void LensGhost::Init()
{
	//ゴーストを書くレンダリングターゲットの初期化
	m_ghostTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	//書いたゴーストにブラーを掛けるための初期化
	m_gaussianBlur.Init(&m_ghostTarget.GetRenderTargetTexture());

	//作成するゴーストの画像の初期化
	SpriteInitData lensGhostData;
	lensGhostData.m_width = 1280;
	lensGhostData.m_height = 720;
	lensGhostData.m_fxFilePath = "Assets/shader/LensGhost.fx";
	lensGhostData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();
	lensGhostData.m_alphaBlendMode = AlphaBlendMode_Add;
	lensGhostData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_lensGhost.Init(lensGhostData);
}

void LensGhost::Render(RenderContext& rc, RenderTarget& target)
{
	//レンズゴーストを書くターゲットに設定
	rc.WaitUntilToPossibleSetRenderTarget(m_ghostTarget);
	rc.SetRenderTargetAndViewport(m_ghostTarget);
	rc.ClearRenderTargetView(m_ghostTarget);
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::lensGhost);
	//太陽だけを描画
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_ghostTarget);
	//通常描画モードに直す
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	//太陽だけが映っているターゲットにブラーを掛ける
	m_gaussianBlur.ExecuteOnGPU(rc, 100);

	//メインとなるターゲットにレンズゴーストを上書きする。
	rc.WaitUntilToPossibleSetRenderTarget(target);
	rc.SetRenderTargetAndViewport(target);
	m_lensGhost.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(target);
}