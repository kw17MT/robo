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
	//PhysicsWorld::CreateInstance();
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




	CaptureStateManager::CreateInstance();
	Player* player;
	player = NewGO<Player>(0,"player");

	Ground* ground = nullptr;
	ground = NewGO<Ground>(0);

	SkyCube* skyCube = nullptr;
	skyCube = NewGO<SkyCube>(0);

	Enemy* enemy[3] = { nullptr };
	enemy[0] = NewGO<Enemy>(0);
	enemy[1] = NewGO<Enemy>(0);
	enemy[1]->SetPosition({ 1000.0f,1000.0f,10000.0f });
	enemy[2] = NewGO<Enemy>(0);
	enemy[2]->SetPosition({ 10000.0f,3000.0f,30000.0f });

	UI* ui = nullptr;
	ui = NewGO<UI>(0);

	SkinModelRender* sun;
	sun = NewGO<SkinModelRender>(0);
	sun->Init("Assets/modelData/sun/sun.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);
	sun->SetPosition({ 0.0f,1000000.0f,0.0f });
	sun->SetScale({ 600.0f,600.0f,600.0f });
	sun->SetIsSun();

	g_camera3D->SetFar(1000000.0f);
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
		//オブジェクトのアップデート関数を一気に行う
		GameObjectManager::GetInstance()->ExecuteUpdate();

		//オブジェクトのドローを行う
		RenderingEngine::GetInstance()->Render(renderContext);
		LightManager::GetInstance().UpdateEyePos();


		enemy[1]->SetPosition({ 1000.0f,1000.0f,10000.0f });
		enemy[2]->SetPosition({ 10000.0f,3000.0f,30000.0f });





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

		//ゲームタイムを用いてエフェクトの再生進行度の更新
		EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
		//エフェクトの描画
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	//サウンドエンジンの消去
	CSoundEngine::DeleteInstance();
	//PhysicsWorld::DeleteInstance();
	//RenderingEngine::DeleteInstance();
	LightManager::DeleteInstance();
	return 0;
}