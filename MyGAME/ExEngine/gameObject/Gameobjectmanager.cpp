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

	rootSignature.Init(
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP
	);

	//メインのレンダーターゲットの初期化
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	copyToBufferSpriteData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	copyToBufferSpriteData.m_width = 1280;
	copyToBufferSpriteData.m_height = 720;
	copyToBufferSpriteData.m_fxFilePath = "Assets/shader/spriteHalfAlpha.fx";

	copyToBufferSprite.Init(copyToBufferSpriteData);

	//輝度用のレンダーターゲットの初期化
	luminanceRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
	//輝度用のスプライトデータとスプライトの初期化
	luminanceSpriteData.m_fxFilePath = "Assets/shader/postEffect.fx";
	luminanceSpriteData.m_vsEntryPointFunc = "VSMain";
	luminanceSpriteData.m_psEntryPoinFunc = "PSLuminance";
	luminanceSpriteData.m_width = 1280;
	luminanceSpriteData.m_height = 720;

	luminanceSpriteData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	luminanceSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	luminanceSprite.Init(luminanceSpriteData);

	gaussianBlur.Init(&luminanceRenderTarget.GetRenderTargetTexture());

	//最終表示用の画像の初期化
	finalSpriteData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;

	finalSpriteData.m_fxFilePath = "Assets/shader/spriteHalfAlpha.fx";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	finalSpriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

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
	
	//シャドウマップのレンダリングターゲットに設定する。
	rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	rc.SetRenderTargetAndViewport(shadowMap);
	rc.ClearRenderTargetView(shadowMap);
	m_renderTypes = 1;									//影するよ
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}

	rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);

	
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);
	//rc.ClearRenderTargetView(mainRenderTarget);

	//for (auto& goList : m_gameObjectListArray) {
	//	for (auto& go : goList) {
	//		go->RenderWrapper(rc);
	//	}
	//}

	////モデルが全部ドローできるまで待つ。
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	/*
	////ブルーム用のメンバとかつくってもいいかも

	////輝度用の画像を出力する
	rc.WaitUntilToPossibleSetRenderTarget(luminanceRenderTarget);
	rc.SetRenderTargetAndViewport(luminanceRenderTarget);
	rc.ClearRenderTargetView(luminanceRenderTarget);
	luminanceSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(luminanceRenderTarget);

	//ガウシアンブラーをかける。
	gaussianBlur.ExecuteOnGPU(rc, 40);
	//最終結果となる画像をメインレンダーターゲットに設定して描く
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	*/

	////ガウシアンブラーをかける。
	//gaussianBlur.ExecuteOnGPU(rc, 60);
	////最終結果となる画像をメインレンダーターゲットに設定して描く
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);
	//finalSprite.Draw(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	////ガウシアンブラーをかける。
	//gaussianBlur.ExecuteOnGPU(rc, 80);
	////最終結果となる画像をメインレンダーターゲットに設定して描く
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);
	//finalSprite.Draw(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	////ガウシアンブラーをかける。
	//gaussianBlur.ExecuteOnGPU(rc, 160);
	////最終結果となる画像をメインレンダーターゲットに設定して描く
	//rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//rc.SetRenderTargetAndViewport(mainRenderTarget);
	//finalSprite.Draw(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	////②メインレンダリングターゲットの内容を
	////  フレームバッファにコピー。
	/*rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);

	copyToBufferSprite.Draw(rc);*/

	//正常に戻す/////////////////////////////////////////////////////
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	m_renderTypes = 0;
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

	//モデルのドロー
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}
	//////////////////////////////////////////////////////////////////
}