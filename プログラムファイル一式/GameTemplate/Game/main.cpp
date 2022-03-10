#include "stdafx.h"
#include "system/system.h"
#include "RenderingEngine.h"
#include "CaptureStateManager.h"

#include "effect/Effect.h"

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

	Title* title = NewGO<Title>(0);
	//Game* game = NewGO<Game>(0, "game");

	g_camera3D->SetPosition({ 0.0f, 0.0f, 3000.0f });
	g_camera3D->SetTarget({ 0.0f, 0.0f, 0.0f });

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