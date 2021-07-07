#include "stdafx.h"
#include "system/system.h"
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
	
	//一緒くたにしないと両方のライトの影響を受けなくなる。////////////////////////////////
	//ディレクションライトの正規化と目の位置をカメラの座標にする。
	//g_lig.directionalLight.direction.Normalize();
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
			a.x -= 20.0f;
			b.z -= 20.0f;
			g_camera3D->SetPosition(a);
			//g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			Vector3 a = g_camera3D->GetPosition();
			a.x += 20.0f;
			Vector3 b = g_camera3D->GetTarget();
			b.z += 20.0f;
			g_camera3D->SetPosition(a);
			//g_camera3D->SetTarget(b);
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