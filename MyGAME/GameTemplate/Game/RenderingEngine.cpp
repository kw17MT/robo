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
	m_mainRenderTarget.Create
	(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	m_albedoTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	m_normalTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	m_specAndDepthTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
	m_velocityTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
	m_shadowTarget.Create(	2048,	2048,	1,	1,	DXGI_FORMAT_R32_FLOAT,	DXGI_FORMAT_D32_FLOAT,	clearColor);
};

void RenderingEngine::InitSprites()
{
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = 1280;
	m_mainSpriteData.m_height = 720;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSprite.Init(m_mainSpriteData);

	m_defferedLighting.InitSprite(m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_shadowTarget, m_velocityTarget);
}

void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 4000.0f, 0.0f);
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

void RenderingEngine::Render(RenderContext& rc)
{
	m_mat.currentVPMatrix = g_camera3D->GetViewProjectionMatrix();

	//影を作成する
	//m_shadow.Render(rc, m_shadowTarget);

	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowTarget);
	rc.SetRenderTargetAndViewport(m_shadowTarget);
	rc.ClearRenderTargetView(m_shadowTarget);

	//シャドウの作成を行うモデルのドロー
	//ライトカメラはRendeingEngineで定義し、SkinModelRender内で使用している。
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowTarget);
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	//ディファードライティングを行う。
	m_defferedLighting.Render(rc);
	//メイン画像を作成する。
	DrawInMainRenderTarget(rc);
	//ポストエフェクトをメイン画像に施す。
	m_postEffect.Render(rc, m_mainRenderTarget);

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

	m_mat.prevVPMatrix = g_camera3D->GetViewProjectionMatrix(); 
	//m_prevViewProjMatrix = g_camera3D->GetViewProjectionMatrix();
}