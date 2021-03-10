#include "stdafx.h"
#include "system/system.h"
#include "Level.h"
#include "ModelRender.h"
#include "Light.h"
#include <string>
#include "FontRender.h"
#include "SpriteRender.h"
#include "Guzai.h"
#include "math.h"
#include "ObjectGene.h"
#include "Kitchen.h"
#include "Counter.h"

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
	
	g_lig.DirDirection = { 0.0f,-1.0f,0.0f };
	g_lig.DirDirection.Normalize();
	g_lig.DirColor = { 1.0f,1.0f,1.0f };
	g_lig.eyePos = g_camera3D->GetPosition();
	
	//ポイントライト
	g_lig.ptPosition = { 0.0f, 300.0f,0.0f };
	g_lig.ptColor = { 100.0f, 50.0f,100.0f };
	g_lig.ptRange = 300.0f;

	//スポットライト
	g_lig.spDirection = { 1.0f,-1.0f,0.0f };
	g_lig.spDirection.Normalize();
	
	g_lig.spAngle = Math::DegToRad(30.0f);


	//ライト用
	/*ModelInitData lightdata;
	lightdata.m_tkmFilePath = "Assets/modelData/light.tkm";

	lightdata.m_fxFilePath = "Assets/shader/model.fx";

	lightdata.m_expandConstantBuffer = &g_lig;
	lightdata.m_expandConstantBufferSize = sizeof(g_lig);

	Model Light;
	Light.Init(lightdata);*/

	//文字の描写///////////////////////////////////////////////////////////////////////
	FontRender SCORE01;
	FontRender SCORE02;
	FontRender TIME;
	FontRender timer;
	
	NewGO<FontRender>(0);
	///////////////////////////////////////////////////////////////////////////////////
	//ただの座標が見やすくするためのもの
	NewGO<Kitchen>(0, "kitchen");

	//画像の描写///////////////////////////////////////////////////////////////////////
	SpriteRender* sp[3];
	
	for (int i = 0; i < 3; i++) {
		sp[i] = NewGO<SpriteRender>(0);
		sp[i]->Init("Assets/image/menu.dds", 128, 256);
		Vector3 pos = { -150.0f,-200.0f,0.0f };
		pos.x += i * 150.0f;
		sp[i]->SetPosition(pos);
	}
	//////////////////////////////////////////////////////////////////////////////////////


	//レベル表示用////////////////////////////////////////////////////////////////////////
	Level level;
	level.Init("Assets/level/level001.tkl", [&](ObjectData& objectData) {return false;});
	//////////////////////////////////////////////////////////////////////////////////////


	//プレイヤーのモデルを作成x2//////////////////////////////////////////////////////////
	ModelRender* player[2];
	player[0] = NewGO<ModelRender>(0,"player01");
	player[0]->SetPlayerNo(1);
	player[1] = NewGO<ModelRender>(0,"player02");
	player[1]->SetPlayerNo(2);
	//////////////////////////////////////////////////////////////////////////////////////


	//具材とバフを作成////////////////////////////////////////////////////////////////////
	ObjectGene* generator;
	generator = NewGO<ObjectGene>(1, "gene");
	//////////////////////////////////////////////////////////////////////////////////////
	
	//カウンターの作成////////////////////////////////////////////////////////////////////
	Counter* co;
	co = NewGO<Counter>(0,"counter");
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
		level.Draw();
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

		/*Light.Draw(renderContext);
		Light.UpdateWorldMatrix(g_lig.ptPosition, g_quatIdentity, g_vec3One);*/

	

		//カメラの移動
		float move = g_pad[0]->GetRStickYF() * 30.0f;
		Vector3 camerapos = g_camera3D->GetPosition();
		camerapos.z -= move;
		g_camera3D->SetPosition(camerapos);

		//Light.Draw(renderContext);
		//level.Draw();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

