#include "stdafx.h"
#include "system/system.h"

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
	struct Light
	{
		//Direction
		Vector3 Direction;
		float pad;
		Vector3 Color;
		float pad1;
		Vector3 eyePos;
		float pad2;
		
		//Point
		Vector3 ptPosition;
		float pad3;
		Vector3 ptColor;
		float ptRange;

		//Spot
		Vector3 spDirection;
		float spAngle;
	};

	Light lig;

	lig.Direction.x = 1.0f;
	lig.Direction.y = 0.0f;
	lig.Direction.z = 0.0f;
	lig.Direction.Normalize();

	lig.Color.x = 1.0f;
	lig.Color.y = 1.0f;
	lig.Color.z = 1.0f;

	lig.eyePos = g_camera3D->GetPosition();
	
	//ポイントライト
	lig.ptPosition.x = 0.0f;
	lig.ptPosition.y = 60.0f;
	lig.ptPosition.z = 0.0f;
	
	lig.ptColor.x = 0.0f;
	lig.ptColor.y = 100.0f;
	lig.ptColor.z = 100.0f;

	lig.ptRange = 300.0f;

	//スポットライト
	lig.spDirection.x = 1.0f;
	lig.spDirection.y = -1.0f;
	lig.spDirection.z = 0.0f;
	lig.spDirection.Normalize();
	
	lig.spAngle = Math::DegToRad(30.0f);

	//Unity用
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";
	
	modeldata.m_expandConstantBuffer = &lig;
	modeldata.m_expandConstantBufferSize = sizeof(lig);
	
	Model Unity;
	Unity.Init(modeldata);

	//ステージ用
	modeldata.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model Stage;
	Stage.Init(modeldata);

	//ライト用
	ModelInitData lightdata;
	lightdata.m_tkmFilePath = "Assets/modelData/light.tkm";

	lightdata.m_fxFilePath = "Assets/shader/model.fx";

	lightdata.m_expandConstantBuffer = &lig;
	lightdata.m_expandConstantBufferSize = sizeof(lig);

	Model Light;
	Light.Init(lightdata);

	struct Disco
	{
		Vector3 discoPos;
		float pad4;
		Vector3 discoDir;
		float pad;
		Vector3 discoColor;
		float discoRange;
	};

	Disco d;
	d.discoPos = { 0.0f,150.0f,0.0f };
	d.discoDir = { 0.0f,1.0f,0.0f };
	d.discoColor = { 10.0f,0.0f,10.0f };
	d.discoDir.Normalize();
	d.discoRange = 100.0f;

	ModelInitData discodata;
	discodata.m_tkmFilePath = "Assets/modelData/light.tkm";

	discodata.m_fxFilePath = "Assets/shader/disco.fx";

	discodata.m_expandConstantBuffer = &d;
	discodata.m_expandConstantBufferSize = sizeof(d);

	Model Disco;
	Disco.Init(discodata);


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
		RotY.Apply(lig.spDirection);

		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, lig.spDirection);
		Quaternion rotX;
		rotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * -0.03f);
		rotX.Apply(lig.spDirection);

		lig.ptPosition.x -= g_pad[0]->GetLStickXF() * 5.0f;
		lig.ptPosition.z -= g_pad[0]->GetLStickYF() * 5.0f;

		Light.UpdateWorldMatrix(lig.ptPosition, g_quatIdentity, g_vec3One);
		Disco.UpdateWorldMatrix(d.discoPos, g_quatIdentity, g_vec3One);

		Unity.Draw(renderContext);
		Stage.Draw(renderContext);
		Light.Draw(renderContext);
		Disco.Draw(renderContext);
		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

