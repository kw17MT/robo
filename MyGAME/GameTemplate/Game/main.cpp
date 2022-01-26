#include "stdafx.h"
#include "system/system.h"
#include "effect/Effect.h"

#include "SkinModelRender.h"
#include "RenderingEngine.h"
#include "Player.h"
#include "Ground.h"
#include "SkyCube.h"
#include "Enemy.h"
#include "UI.h"
#include "CaptureStateManager.h"

#include "Game.h"

#include "Title.h"
#include "GameDirector.h"

// ウィンドウプログラムのメイン関数。
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("SkySentinel"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	//レンダリングを統括するインスタンス。
	RenderingEngine::CreateInstance();
	//レンダリングにかかわるものの初期化。
	RenderingEngine::GetInstance()->PrepareRendering();
	//ワールド上のライティング関連のインスタンス。
	LightManager::CreateInstance();
	//サウンドを鳴らす用のインスタンス
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();
	//エフェクトを出すインスタンス
	EffectEngine::CreateInstance();
	//ゲームタイムを測るもの
	CStopwatch stopWatch;

	//Title* title;
	//title = NewGO<Title>(0);
	Game* a = NewGO<Game>(0, "game");

	g_camera3D->SetPosition({ 0.0f, 0.0f, 3000.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });

	//////////////////////////////////////
	// インスタンシングテスト
	//////////////////////////////////////


	//const int width = 50;
	//const int height = 10;
	//const int numHumanModel = width * height;
	//Vector3* humanPos = new Vector3[numHumanModel];

	//int humanNo = 0;
	//for (int x = 0; x < width; x++)
	//{
	//	for (int y = 0; y < height; y++)
	//	{
	//		humanPos[humanNo].x = -2400.0f + 1000.0f * x;
	//		humanPos[humanNo].y = -1250.0f + 2500.0f * y;
	//		humanPos[humanNo].z = 0.0f;

	//		humanNo++;
	//	}
	//}

	//// step-2 500体分のワールド行列関係の各種バッファを確保。
	//Matrix* worldMatrixArray = new Matrix[numHumanModel];

	//StructuredBuffer worldMatrixSB;
	//worldMatrixSB.Init(
	//	sizeof(Matrix),
	//	numHumanModel,
	//	nullptr
	//);

	//ModelInitData modelData;
	//modelData.m_tkmFilePath = "Assets/modelData/robo/robo.tkm";
	//modelData.m_fxFilePath = "Assets/shader/sample3DInstancing.fx";

	//modelData.m_expandShaderResoruceView[0] = &worldMatrixSB;
	//Model model;
	//model.Init(modelData);


	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		if (GameDirector::GetInstance().GetGameScene() == enExit)
		{
			break;
		}
		//レンダリング開始。
		g_engine->BeginFrame();
		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		//オブジェクトのアップデート関数を一気に行う
		GameObjectManager::GetInstance()->ExecuteUpdate();

		// インスタンシングテスト
		//Quaternion qRot = Quaternion::Identity;

		//for (int i = 0; i < numHumanModel; i++)
		//{
		//	worldMatrixArray[i] = model.CalcWorldMatrix(humanPos[i], qRot, g_vec3One);
		//}
		//// step-5 ワールド行列の内容をグラフィックメモリにコピー。
		//worldMatrixSB.Update(worldMatrixArray);

		//// step-6 人物ののモデルをインスタンシグ描画。
		//model.DrawInstancing(renderContext, numHumanModel);
		////////////////////////////////////////////////////////////////////////////////////////////

		//オブジェクトのドローを行う
		RenderingEngine::GetInstance()->Render(renderContext);
		LightManager::GetInstance().UpdateEyePos();

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

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	//サウンドエンジンの消去
	CSoundEngine::DeleteInstance();
	PhysicsWorld::DeleteInstance();
	//RenderingEngine::DeleteInstance();
	LightManager::DeleteInstance();
	return 0;
}