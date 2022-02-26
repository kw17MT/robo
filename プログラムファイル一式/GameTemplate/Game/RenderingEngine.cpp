#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::instance = nullptr;

namespace
{
	const Vector3 INITIAL_LIGHTCAMERA_POS = { 0.0f, 10000.0f, 0.0f };	//ライトカメラの初期位置
	const int DEFAULT_WIDTH = 1280;										//通常のテクスチャ横幅
	const int DEFAULT_HEIGHT = 720;										//通常のテクスチャ縦幅
	const float LIGHTCAMERA_FAR = 100000.0f;							//ライトカメラのファークリップ
	const int LIGHTCAMERA_WIDTH = 500;									//ライトカメラが映す横幅
	const int LIGHTCAMERA_HEIGHT = 400;									//ライトカメラが映す縦幅
}

void RenderingEngine::PrepareRendering()
{
	//レンダーターゲットをすべて作成する
	InitRenderTargets();
	//スプライトの初期化
	InitSprites();
	//ライトカメラの初期化
	InitLightCamera();
	//ポストエフェクトで使うレンダーターゲットの送信
	m_postEffect.Init(m_mainRenderTarget, m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_velocityTarget);
}

void RenderingEngine::InitRenderTargets()
{
	//メインゲームの画面となるターゲット
	m_mainRenderTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//アルベドマップ書き込みのターゲット
	m_albedoTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//法線マップ書き込みのターゲット
	m_normalTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	//鏡面反射率と深度値書き込みのターゲット
	m_specAndDepthTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//ベロシティマップ書き込みのターゲット
	m_velocityTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//シャドウ書き込みのターゲットの初期化
	m_shadow.InitShadowTarget();
};

void RenderingEngine::InitSprites()
{
	//メインレンダーターゲットを使った画像の初期化
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = DEFAULT_WIDTH;
	m_mainSpriteData.m_height = DEFAULT_HEIGHT;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_mainSprite.Init(m_mainSpriteData);

	//ディファードレンダリングでそれぞれのターゲットにマップを書き込む
	m_defferedLighting.InitSprite(m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_shadow.GetShadowMap(), m_velocityTarget);
}

void RenderingEngine::InitLightCamera()
{
	//ライトカメラの座標設定
	m_lightCamera.SetPosition(INITIAL_LIGHTCAMERA_POS);
	//ライトカメラの映す限界
	m_lightCamera.SetFar(LIGHTCAMERA_FAR);
	//ライトカメラのターゲット
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//カメラの上をX座標にしておく
	m_lightCamera.SetUp({ 1, 0, 0 });							
	//ライトカメラを180度回転させて地面を見るようにする
	m_lightCamera.SetViewAngle(Math::DegToRad(180.0f));
	//平行投影にする
	m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//ライトカメラが作る影の範囲設定
	m_lightCamera.SetWidth(LIGHTCAMERA_WIDTH);
	m_lightCamera.SetHeight(LIGHTCAMERA_HEIGHT);
	//設定を更新する
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

void RenderingEngine::DrawUI(RenderContext& rc)
{
	//メインのゲーム画面にUIを書き込む
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

	//ディファードライティングを行う。
	m_defferedLighting.Render(rc);

	//メインの最終表示となる画像レンダリングターゲットにディファードライティング画像を描画
	DrawInMainRenderTarget(rc);

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