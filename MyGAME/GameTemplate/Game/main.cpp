#include "stdafx.h"
#include "system/system.h"

#include "Light.h"
#include "Game.h"

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

	//一緒くたにしないと両方のライトの影響を受けなくなる。////////////////////////////////
	//ディレクションライトの正規化と目の位置をカメラの座標にする。
	g_lig.DirDirection.Normalize();
	g_lig.eyePos = g_camera3D->GetPosition();
	//////////////////////////////////////////////////////////////////////////////////////
	
	//プレイヤー、文字、オブジェクトなどの生成////////////////////////////////////////////
	NewGO<Game>(0, "game");
	//////////////////////////////////////////////////////////////////////////////////////


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

			camerapos.y -= move;
			g_camera3D->SetPosition(camerapos);
		}
		if (g_pad[0]->GetRStickXF()) {
			float move = g_pad[0]->GetRStickXF() * 30.0f;
			Vector3 camerapos = g_camera3D->GetPosition();

			camerapos.x -= move;
			g_camera3D->SetPosition(camerapos);
		}

		

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

