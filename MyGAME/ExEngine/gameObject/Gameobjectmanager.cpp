/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
		std::abort();
	}
	m_instance = this;

	//メインのレンダーターゲットの初期化
	//最終のレンダリングターゲットにする
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//最終表示用の画像データ
	finalSpriteWithFXAAData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	finalSpriteWithFXAAData.m_width = 1280;
	finalSpriteWithFXAAData.m_height = 720;
	finalSpriteWithFXAAData.m_fxFilePath = "Assets/shader/spriteHalfAlpha.fx";

	finalSpriteWithFXAA.Init(finalSpriteWithFXAAData);

	//シャドウのオフスクリーンレンダリング作成
	shadowMap.Create(
		2048,
		2048,
		1,
		1,
		//DXGI_FORMAT_R8G8B8A8_UNORM,			//投影シャドウで使う
		DXGI_FORMAT_R32_FLOAT,					//デプスシャドウで使う
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	//ライトカメラの作成
	lightCamera.SetPosition(0.0f, 1000.0f, -500.0f);
	lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	lightCamera.SetUp({ 1, 0, 0});							//カメラの上をX座標にしておく
	lightCamera.SetViewAngle(Math::DegToRad(145.0f));
	//平行投影にする場合
	/*
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1280);
	lightCamera.SetHeight(720);
	*/
	lightCamera.Update();



	//被写界深度用レンダーターゲット
	/*depthInViewMap.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

	depthGaussian.Init(&mainRenderTarget.GetRenderTargetTexture());*/

	//被写界深度込みの合成画像
	//combineDepthSpriteData.m_textures[0] = &depthGaussian.GetBokeTexture();//&gaussianBlur[0].GetBokeTexture();
	//combineDepthSpriteData.m_textures[1] = &depthInViewMap.GetRenderTargetTexture();
	//combineDepthSpriteData.m_width = 1280;
	//combineDepthSpriteData.m_height = 720;
	//combineDepthSpriteData.m_fxFilePath = "Assets/shader/depthInView.fx";
	//combineDepthSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//combineDepthSpriteData.m_alphaBlendMode = AlphaBlendMode_Trans;
	//depthInViewSprite.Init(combineDepthSpriteData);

	//depthTargets[] = { &mainRenderTarget, &depthInViewMap };


	albedoMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	normalMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	worldPosMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);

	defferedSpriteData.m_width = 1280;
	defferedSpriteData.m_height = 720;
	defferedSpriteData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[2] = &worldPosMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[3] = &shadowMap.GetRenderTargetTexture();
	defferedSpriteData.m_fxFilePath = "Assets/shader/defferedSprite.fx";
	defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	defferedSprite.Init(defferedSpriteData);

	m_postEffect.Init(mainRenderTarget);
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//死亡フラグがついているゲームオブジェクトを破棄する。
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	//①メインレンダリングターゲットに切り替える
	//　レンダリングターゲットはメンバ変数にある
	//　コンストラクタで初期化。				→フォーマットの違いでERRORがでるかもしれない。それぞれのクラスで同じフォーマットで初期化する。
	
	/*シャドウマップ作成************************************************************************/
	//シャドウマップのレンダリングターゲットに設定する。
	rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	rc.SetRenderTargetAndViewport(shadowMap);
	rc.ClearRenderTargetView(shadowMap);
	m_renderTypes = enRenderShade;									//影するよ
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}
	rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	/********************************************************************************************/

	/*ディファード作成*****************************************************************************/
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(defferedTargets), defferedTargets);
	m_renderTypes = enRenderNormal;
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	/********************************************************************************************/

	/*ここから最終的に表示する画面（画像）を作成*************************************************/
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	rc.ClearRenderTargetView(mainRenderTarget);
	defferedSprite.Draw(rc);

	//UIやポストエフェクトの掛けたくない画像を最前面にドロー
	/*m_renderTypes = enRenderUI;
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}*/

	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	/********************************************************************************************/
	
	//ブルームとAAを行う
	m_postEffect.Render(rc, mainRenderTarget);

	/*現在のレンダーターゲットをフレームバッファにコピー*****************************************/
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//描画するサイズを設定
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	finalSpriteWithFXAA.Draw(rc);
	//最終の画面を表示
	//fxaaSprite.Draw(rc);
}