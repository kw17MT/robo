#include "stdafx.h"
#include "system/system.h"

#include "Light.h"
#include "Game.h"
#include "Title.h"


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

	//一緒くたにしないと両方のライトの影響を受けなくなる。////////////////////////////////
	//ディレクションライトの正規化と目の位置をカメラの座標にする。
	g_lig.directionalLight.direction.Normalize();
	//g_lig.directionalLight.color = { 5.0f, 5.0f, 5.0f };
	//ポストエフェクトのテスト用。後から消すこと
	g_postLig.directionalLight.direction.Normalize();
	//g_postLig.directionalLight.direction.y = -1.0f
	g_postLig.directionalLight.color = { 20.0f, 20.0f, 20.0f };
	//////////////////////////////////////////////////////////////////////////////////////
	
	//プレイヤー、文字、オブジェクトなどの生成////////////////////////////////////////////
	//NewGO<Game>(0, "game");
	NewGO<Title>(0, "title");
	//////////////////////////////////////////////////////////////////////////////////////
	
	
	

	////影描画用のライトカメラを作成する。			〇
	//Camera lightCamera;
	////カメラの位置を設定。これはライトの位置。
	//lightCamera.SetPosition(0, 500, 0);
	////カメラの注視点を設定。これがライトが照らしている場所。
	//lightCamera.SetTarget(0, 0, 0);
	////上方向を設定。今回はライトが真下を向いているので、X方向を上にしている。
	//lightCamera.SetUp({ 1, 0, 0 });
	////ライトビュープロジェクション行列を計算している。
	//lightCamera.Update();


	////シャドウマップ描画用のレンダリングターゲットを作成する。			〇
	//float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//RenderTarget shadowMap;
	//shadowMap.Create(
	//	1024,
	//	1024,
	//	1,
	//	1,
	//	DXGI_FORMAT_R8G8B8A8_UNORM,
	//	DXGI_FORMAT_D32_FLOAT,
	//	clearColor
	//);

	////シャドウマップに描画するモデルを初期化する。		〇
	//ModelInitData teapotShadowModelInitData;
	////シャドウマップ描画用のシェーダーを指定する。
	//teapotShadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
	//teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//Model teapotShadowModel;
	//teapotShadowModel.Init(teapotShadowModelInitData);
	//teapotShadowModel.UpdateWorldMatrix(
	//	{ 0, 50, 0 },
	//	g_quatIdentity,
	//	g_vec3One
	//);


	///*　	×
	//ここまで用意できたもので影落とすためにmodel.Draw(rc, camera)をする。
	//modelを上から見たときの灰色画像をshadowTargetを用いて移す。*/



	////通常描画のティーポットモデルを初期化。
	//Model teapotModel;
	//teapotShadowModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//teapotShadowModelInitData.m_expandConstantBuffer = &g_lig;
	//teapotShadowModelInitData.m_expandConstantBufferSize = sizeof(g_lig);
	//teapotModel.Init(teapotShadowModelInitData);
	//teapotModel.UpdateWorldMatrix(
	//	{ 0, 50, 0 },
	//	g_quatIdentity,
	//	g_vec3One
	//);

	////step-1 影を受ける背景モデルを初期化。			
	//ModelInitData bgModelInitData;
	////シャドウレシーバー(影が落とされるモデル)用のシェーダーを指定する。
	//bgModelInitData.m_fxFilePath = "Assets/shader/ShadowReciever.fx";
	////シャドウマップを拡張SRVに設定する。
	//bgModelInitData.m_expandShaderResoruceView = &shadowMap.GetRenderTargetTexture();
	////ライトビュープロジェクション行列を拡張定数バッファに設定する。
	//bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	//bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	//bgModelInitData.m_tkmFilePath = "Assets/modelData/Floor.tkm";

	//Model bgModel;
	//bgModel.Init(bgModelInitData);


	

	
	
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
		
		/*Quaternion RotY;
		RotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.03f);
		RotY.Apply(g_lig.spDirection);

		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, g_lig.spDirection);
		Quaternion rotX;
		rotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * -0.03f);
		rotX.Apply(g_lig.spDirection);*/

		//カメラの移動
		if (g_pad[0]->GetRStickYF()) {
			float move = g_pad[0]->GetRStickYF() * 30.0f;
			Vector3 camerapos = g_camera3D->GetPosition();
			Vector3 cameraTarget = g_camera3D->GetTarget();

			camerapos.z -= move;
			g_camera3D->SetPosition(camerapos);

			cameraTarget.z -= move;
			g_camera3D->SetTarget(cameraTarget);
		}
		if (g_pad[0]->GetRStickXF()) {
			float move = g_pad[0]->GetRStickXF() * 30.0f;
			Vector3 camerapos = g_camera3D->GetPosition();
			Vector3 cameraTarget = g_camera3D->GetTarget();

			camerapos.x -= move;
			g_camera3D->SetPosition(camerapos);
			cameraTarget.x -= move;
			g_camera3D->SetTarget(cameraTarget);
		}
		if (g_pad[0]->IsPress(enButtonSelect)) {
			float move = 30.0f;
			Vector3 camerapos = g_camera3D->GetPosition();
		
			camerapos.y -= move;
			g_camera3D->SetPosition(camerapos);
		}
		if (g_pad[0]->IsPress(enButtonStart)) {
			float move = 30.0f;
			Vector3 camerapos = g_camera3D->GetPosition();

			camerapos.y += move;
			g_camera3D->SetPosition(camerapos);
		}







		/*renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
		renderContext.SetRenderTargetAndViewport(shadowMap);
		renderContext.ClearRenderTargetView(shadowMap);


		teapotShadowModel.Draw(renderContext, lightCamera);


		renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);


		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		renderContext.SetViewport(g_graphicsEngine->GetFrameBufferViewport());


		teapotModel.Draw(renderContext);

		bgModel.Draw(renderContext);*/






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

