#include "stdafx.h"
#include "system/system.h"
#include "Level.h"
#include "ModelRender.h"
#include "Light.h"
#include <string>


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

	//ディレクションライト、ポイントライト
	//一緒くたにしないと両方のライトの影響を受けなくなる。
	
	g_lig.DirDirection = { -1.0f,1.0f,0.0f };
	g_lig.DirDirection.Normalize();
	g_lig.DirColor = { 1.0f,1.0f,1.0f };
	g_lig.eyePos = g_camera3D->GetPosition();
	
	//ポイントライト
	g_lig.ptPosition = { 0.0f, 60.0f,0.0f };
	g_lig.ptColor = { 0.0f, 100.0f,100.0f };
	g_lig.ptRange = 300.0f;

	//スポットライト
	g_lig.spDirection = { 1.0f,-1.0f,0.0f };
	g_lig.spDirection.Normalize();
	
	g_lig.spAngle = Math::DegToRad(30.0f);


	//ライト用
	//ModelInitData lightdata;
	//lightdata.m_tkmFilePath = "Assets/modelData/light.tkm";

	//lightdata.m_fxFilePath = "Assets/shader/model.fx";

	//lightdata.m_expandConstantBuffer = &g_lig;
	//lightdata.m_expandConstantBufferSize = sizeof(g_lig);

	//Model Light;
	//Light.Init(lightdata);
	/// レベル表示用
	Level level;
	level.Init("Assets/level/level001.tkl", [&](ObjectData& objectData) {return false;});

	ModelRender* re[2];
	re[0] = NewGO<ModelRender>(0);
	re[0]->SetPlayerNo(1);
	re[1] = NewGO<ModelRender>(0);
	re[1]->SetPlayerNo(2);

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

		Quaternion RotY;
		RotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.03f);
		RotY.Apply(g_lig.spDirection);

		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, g_lig.spDirection);
		Quaternion rotX;
		rotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * -0.03f);
		rotX.Apply(g_lig.spDirection);

		//g_lig.ptPosition.x -= g_pad[0]->GetLStickXF() * 5.0f;
		//g_lig.ptPosition.z -= g_pad[0]->GetLStickYF() * 5.0f;

		//Light.UpdateWorldMatrix(g_lig.ptPosition, g_quatIdentity, g_vec3One);

		//文字を出力する。
		

		//カメラの移動
		float move = g_pad[0]->GetRStickYF() * 30.0f;
		Vector3 camerapos = g_camera3D->GetPosition();
		camerapos.z -= move;
		g_camera3D->SetPosition(camerapos);

		//Light.Draw(renderContext);
		level.Draw();
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

