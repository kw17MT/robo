#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::instance = nullptr;

void RenderingEngine::PrepareRendering()
{
	InitRenderTargets();
	InitSprites();
	InitLightCamera();
	m_postEffect.Init(m_mainRenderTarget, m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_velocityTarget);
}

void RenderingEngine::InitRenderTargets()
{
	m_mainRenderTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_albedoTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_normalTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	m_specAndDepthTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_velocityTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_shadow.InitShadowTarget();
};

void RenderingEngine::InitSprites()
{
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = 1280;
	m_mainSpriteData.m_height = 720;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_mainSprite.Init(m_mainSpriteData);

	m_defferedLighting.InitSprite(m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_shadow.GetShadowMap(), m_velocityTarget);
	m_shadow.InitCascade(m_mainRenderTarget, m_normalTarget, m_specAndDepthTarget);
}

void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 10000.0f, 0.0f);
	m_lightCamera.SetFar(100000.0f);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1, 0, 0 });							//カメラの上をX座標にしておく
	m_lightCamera.SetViewAngle(Math::DegToRad(120.0f));
	m_lightCamera.Update();
}

void RenderingEngine::DrawInMainRenderTarget(RenderContext& rc)
{
	//最終出力のメインレンダーターゲットに書き込み開始
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);
	//ディファードライティングされたメインの画像を合成。
	m_defferedLighting.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::StartForwardRendering(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTarget(
		m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
		m_albedoTarget.GetDSVCpuDescriptorHandle()
	);
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//ディファードライティングされたメインの画像を合成。
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::DrawUI(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::ui);
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::Render(RenderContext& rc)
{
	//カメラのプロジェクションビュー行列を取得
	m_mat.currentVPMatrix = g_camera3D->GetViewProjectionMatrix();

	//影を作成する
	m_shadow.Render(rc);

	//m_shadow.RenderCascade(rc);

	//ディファードライティングを行う。
	m_defferedLighting.Render(rc);

	//メインの最終表示となる画像レンダリングターゲットにディファードライティング画像を描画
	DrawInMainRenderTarget(rc);

	//ディファードライティング画像に空、太陽をフォワード的に描画
	StartForwardRendering(rc);

	//ポストエフェクトをメイン画像に施す。
	m_postEffect.Render(rc, m_mainSprite, m_mainRenderTarget);

	//最後にUIを描画する
	DrawUI(rc);

	/*現在のレンダーターゲットをフレームバッファにコピー*****************************************/
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//描画するサイズを設定
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//出来た画像の表示
	m_mainSprite.Draw(rc);

	//カメラの1フレーム前のプロジェクションビュー行列を取得
	m_mat.prevVPMatrix = g_camera3D->GetViewProjectionMatrix(); 
}