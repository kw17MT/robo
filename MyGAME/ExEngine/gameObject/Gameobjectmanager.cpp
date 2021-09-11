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
	finalSpriteData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;
	finalSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";

	finalSprite.Init(finalSpriteData);

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


	albedoMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	normalMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	specAndDepthMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);

	defferedSpriteData.m_width = 1280;
	defferedSpriteData.m_height = 720;
	defferedSpriteData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[3] = &shadowMap.GetRenderTargetTexture();
	defferedSpriteData.m_fxFilePath = "Assets/shader/deffered/defferedSprite.fx";
	defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	defferedSprite.Init(defferedSpriteData);

	forwardBloomTarget.Create(
		1280,   // 解像度はメインレンダリングターゲットと同じ
		720,  // 解像度はメインレンダリングターゲットと同じ
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	m_forwardBloom.Init(forwardBloomTarget);
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
	//シャドウの作成を行うモデルのドロー
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	/********************************************************************************************/

	/*ディファード作成*****************************************************************************/
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(defferedTargets), defferedTargets);
	m_renderTypes = enRenderNormal;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	/********************************************************************************************/

	/*ここから最終的に表示する画面（画像）を作成*************************************************/
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	rc.ClearRenderTargetView(mainRenderTarget);
	//ディファードライティングをした画像の描画
	defferedSprite.Draw(rc);
	//m_forwardBloom.Render(rc, mainRenderTarget);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	/********************************************************************************************/

	/*フォワードレンダリングでのブルーム*/
	rc.WaitUntilToPossibleSetRenderTarget(forwardBloomTarget);
	rc.SetRenderTargetAndViewport(forwardBloomTarget);
	rc.ClearRenderTargetView(forwardBloomTarget);
	m_renderTypes = enRenderLuminance;									//ブラー
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(forwardBloomTarget);
	m_forwardBloom.Render(rc, mainRenderTarget);
	/********************************************************************************************/
	
	/*ポストエフェクトを行う*********************************************************************/
	//ブルームとAAを行う
	m_postEffect.Render(rc, mainRenderTarget);
	/********************************************************************************************/

	/*UIやポストエフェクトの掛けたくない画像を最前面にドロー*************************************/
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	m_renderTypes = enRenderUI;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	/********************************************************************************************/

	/*現在のレンダーターゲットをフレームバッファにコピー*****************************************/
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//描画するサイズを設定
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//最終の画面を表示
	finalSprite.Draw(rc);

}