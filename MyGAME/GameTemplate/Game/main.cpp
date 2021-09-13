#include "stdafx.h"
#include "system/system.h"
#include "effect/Effect.h"

#include "SkinModelRender.h"
#include "RenderingEngine.h"

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
	
	RenderingEngine::CreateInstance();
	RenderingEngine::GetInstance()->PrepareRendering();// InitRenderTargets();
	//RenderingEngine::GetInstance()->InitSprites();
	LightManager::CreateInstance();
	//サウンドを鳴らす用のインスタンス
	CSoundEngine::CreateInstance();
	CSoundEngine::GetInstance()->Init();

	//エフェクトを出すインスタンス
	EffectEngine::CreateInstance();

	//ゲームタイムを測るもの
	CStopwatch stopWatch;

	SkinModelRender* test[2] = { nullptr };
	test[0] = NewGO<SkinModelRender>(0);
	test[1] = NewGO<SkinModelRender>(0);
	test[0]->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f });
	test[0]->SetPosition({ 0.0f,0.0f,-1000.0f });
	test[1]->Init("Assets/modelData/testFish.tkm", "Assets/modelData/testFish.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f });

	AnimationClip anim[2];
	anim[0].Load("Assets/modelData/robo.tka");
	anim[0].SetLoopFlag(true);
	anim[1].Load("Assets/modelData/testFish.tka");
	anim[1].SetLoopFlag(true);
	/*test[0]->InitAnimation(&anim[0], 1);
	test[0]->PlayAnimation(1, 1);*/
	test[1]->InitAnimation(&anim[1], 1);
	test[1]->PlayAnimation(1, 1);
	

		
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
		//GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		RenderingEngine::GetInstance()->Render(renderContext);
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
			b.x -= 20.0f;
			g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonDown)) {
			Vector3 a = g_camera3D->GetPosition();
			a.x += 20.0f;
			Vector3 b = g_camera3D->GetTarget();
			b.x += 20.0f;
			g_camera3D->SetPosition(a);
			g_camera3D->SetTarget(b);
		}
		if (g_pad[0]->IsPress(enButtonX)) {
			Vector3 a = g_camera3D->GetPosition();
			a.z -= 20.0f;
			g_camera3D->SetPosition(a);
		}
		if (g_pad[0]->IsPress(enButtonY)) {
			Vector3 a = g_camera3D->GetPosition();
			a.z += 20.0f;
			g_camera3D->SetPosition(a);
			test[1]->PlayAnimation(1, 1);
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
	return 0;
}