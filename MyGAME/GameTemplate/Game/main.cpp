#include "stdafx.h"
#include "system/system.h"

#include "Light.h"
#include "Game.h"
#include "Title.h"
#include "effect/Effect.h"

// ウィンドウプログラムのメイン関数。
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//サウンドを鳴らす用のインスタンス
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	//エフェクトを出すインスタンス
	EffectEngine::CreateInstance();

	//ゲームタイムを測るもの
	CStopwatch stopWatch;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 影描画用のライトカメラを作成する
	Camera lightCamera;
	lightCamera.SetPosition(0, 1500, 0);
	lightCamera.SetTarget(0, 0, 0);
	lightCamera.SetUp({ 1, 0, 0 });
	lightCamera.Update();
	// シャドウマップ描画用のレンダリングターゲットを作成する
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	RenderTarget shadowMap;
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	//RenderTarget a = *GameObjectManager::GetInstance()->GetShadowMap();
	//Camera b = *GameObjectManager::GetInstance()->GetLightCamera();

	// シャドウマップに描画するモデルを初期化する
	ModelInitData shadowTestData;
	shadowTestData.m_fxFilePath = "Assets/shader/drawShadowMap.fx";
	shadowTestData.m_tkmFilePath = "Assets/modelData/Chef/ChefRed/Chef01.tkm";

	Model shadowTest;
	shadowTest.Init(shadowTestData);
	shadowTest.UpdateWorldMatrix(
		{ 0, 50, 0 },
		g_quatIdentity,
		g_vec3One
	);

	// step-1 影を受ける背景モデルを初期化
	ModelInitData bgModelData;
	bgModelData.m_tkmFilePath = "Assets/modelData/floor/floor_red.tkm";
	bgModelData.m_fxFilePath = "Assets/shader/shadowReciever.fx";
	//書き終えた影のテクスチャを持っていく
	bgModelData.m_expandShaderResoruceView = /*&a.GetRenderTargetTexture();*/&shadowMap.GetRenderTargetTexture();
	bgModelData.m_expandConstantBuffer = (void*)/*&b.GetViewProjectionMatrix();*/&lightCamera.GetViewProjectionMatrix();
	bgModelData.m_expandConstantBufferSize = sizeof(/*b.GetViewProjectionMatrix()*/lightCamera.GetViewProjectionMatrix());

	Model floor;
	floor.Init(bgModelData);

	
	//一緒くたにしないと両方のライトの影響を受けなくなる。////////////////////////////////
	//ディレクションライトの正規化と目の位置をカメラの座標にする。
	g_lig.directionalLight.direction.Normalize();
	//////////////////////////////////////////////////////////////////////////////////////
	
	//プレイヤー、文字、オブジェクトなどの生成////////////////////////////////////////////
	NewGO<Title>(0, "title");
		
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		g_lig.eyePos = g_camera3D->GetPosition();
		g_postLig.eyePos = g_camera3D->GetPosition();

		//ライトカメラをアップデートしてみてる
		//GameObjectManager::GetInstance()->GetLightCamera()->Update();

		//////////////////////////////////////////////////////////////////////////////////////////////////


		//renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
		//renderContext.SetRenderTargetAndViewport(shadowMap);
		//renderContext.ClearRenderTargetView(shadowMap);

		//// 影モデルを描画
		//shadowTest.Draw(renderContext, lightCamera);

		//// 書き込み完了待ち
		//renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);

		//// 通常レンダリング
		//// レンダリングターゲットをフレームバッファーに戻す
		//renderContext.SetRenderTarget(
		//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		//);
		//renderContext.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//// step-2 影を受ける背景を描画
		//floor.Draw(renderContext);



		//カメラの移動
		if (g_pad[0]->IsPress(enButtonLeft)) {
			Vector3 a = g_camera3D->GetPosition();
			a.y -= 20.0f;
			g_camera3D->SetPosition(a);			
		}
		if (g_pad[0]->IsPress(enButtonRight)) {
			Vector3 a = g_camera3D->GetPosition();
			a.y += 20.0f;
			g_camera3D->SetPosition(a);
		}
		if (g_pad[0]->IsPress(enButtonUp)) {
			Vector3 a = g_camera3D->GetPosition();
			Vector3 b = g_camera3D->GetTarget();
			a.z -= 20.0f;
			b.z -= 20.0f;
			g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			Vector3 a = g_camera3D->GetPosition();
			a.z += 20.0f;
			Vector3 b = g_camera3D->GetTarget();
			b.z += 20.0f;
			g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}

		//スピンロックを行う。
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = 16 - (int)stopWatch.GetElapsedMillisecond();
		} while (restTime > 0);
		//ストップウォッチの計測終了
		stopWatch.Stop();
		//デルタタイムをストップウォッチの計測時間から、計算する
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());

		EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	CSoundEngine::DeleteInstance();
	return 0;
}

