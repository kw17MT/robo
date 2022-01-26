#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"
#include "Rader.h"
#include "SoundSource.h"
#include "EliminateTelop.h"
#include "ObjectiveEnemyNum.h"
#include "Title.h"
#include "ClearTelop.h"
#include "Fade.h"
#include "GameDirector.h"
#include "CaptureStateManager.h"
#include "AfterGameOverScene.h"
#include "EnemyRepopManager.h"

namespace
{
	const int OBJECTIVE_NUM = 30;
	const float BGM_DECREASE_VOLUME = 0.001f;
	const float FADE_START_VOLUME = 24.0f;
}

Game::~Game()
{
	//プレイヤーインスタンス削除
	DeleteGO(m_player); m_player = nullptr;
	//敵生成器インスタンス削除
	DeleteGO(m_enemyGenerator); m_enemyGenerator = nullptr;
	//スカイキューブマップインスタンス削除
	DeleteGO(m_sky); m_sky = nullptr;
	//地面インスタンス削除
	DeleteGO(m_ground); m_ground = nullptr;
	//UIインスタンス削除
	DeleteGO(m_ui); m_ui = nullptr;
	//太陽インスタンス削除
	DeleteGO(m_sun); m_sun = nullptr;
	//レーダーインスタンス削除
	DeleteGO(m_rader); m_rader = nullptr;
	//目標撃破数表示インスタンス削除
	DeleteGO(m_objectiveEnemyNum);
	//BGMインスタンス削除
	DeleteGO(m_bgm);
	//クリア表示インスタンス削除
	DeleteGO(m_clear);
	//フラグ管理インスタンス削除
	CaptureStateManager::DeleteInstance();
	EnemyRepopManager::DeleteInstance();
}

bool Game::Start()
{
	//敵再生成マネージャーの作成
	EnemyRepopManager::CreateInstance();
	EnemyRepopManager::GetInstance().ResetParam();

	//プレイヤー生成
	m_player = NewGO<Player>(0,"player");
	//スカイキューブマップ作成
	m_sky = NewGO<SkyCube>(0);
	//地面作成
	m_ground = NewGO<Ground>(0);
	//太陽の作成
	m_sun = NewGO<Sun>(0);
	//レーダーの作成
	m_rader = NewGO<Rader>(0, "rader");
	//UIの作成
	m_ui = NewGO<UI>(0);
	//全滅させよのテロップ
	m_eliminateTelop = NewGO<EliminateTelop>(0);
	//残り敵数を表示する
	m_objectiveEnemyNum = NewGO<ObjectiveEnemyNum>(0, "objective");
	//ゲームクリアとなる撃破数の設定
	m_objectiveEnemyNum->SetObjectiveNum(OBJECTIVE_NUM);
	//敵生成器作成
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	//BGM作成
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/bgm1.wav", false);
	m_bgm->SetVolume(m_bgmVolume);
	m_bgm->Play(true);

	//ゲーム中シーンに遷移する
	GameDirector::GetInstance().SetGameScene(enInGame);
	//ステートマネージャーの作成
	CaptureStateManager::CreateInstance();


	return true;
}

void Game::Update()
{
	//レーダーにプレイヤーの位置を与える
	m_rader->SetPlayerPos(m_player->GetRoboPosition());
	//レーダーに生成した敵の数を与える
	m_rader->SetEnemyNum(m_enemyGenerator->GetEnemyNum());
	//敵の数分その位置座標を保存、更新
	for (int i = 0; i < m_enemyGenerator->GetEnemyNum(); i++)
	{
		//敵の位置座標を設定
		Vector4 enemyInfo = m_enemyGenerator->GetEnemyPos(i);
		//敵が生きているかどうかをwに入れる
		enemyInfo.w = m_enemyGenerator->GetIsEnemyAlive(i);
		//上記情報をレーダーに渡す
		m_rader->SaveEnemyPosAndIsAlive(i, enemyInfo);
	}

	//ゲームクリアシーンの時
	if (GameDirector::GetInstance().GetGameScene() == enGameClear)
	{
		//クリア後の経過時間を測定
		m_colapsedTimeAfterClear += GameTime().GetFrameDeltaTime();
		//徐々にBGMの大きさを減らす
		m_bgmVolume -= BGM_DECREASE_VOLUME;
		//0未満にならないようにする
		if (m_bgmVolume < 0.0f)
		{
			m_bgmVolume = 0.0f;
		}
		//BGM音量を設定
		m_bgm->SetVolume(m_bgmVolume);

		//クリアサウンドを鳴らしていなかったら
		if (!m_isSoundClear)
		{
			//クリアサウンドを鳴らす
			CSoundSource* clearSE = NewGO<CSoundSource>(0);
			clearSE->Init(L"Assets/sound/clear.wav", false);
			clearSE->SetVolume(1.0f);
			clearSE->Play(false);
			m_isSoundClear = true;
		}
		//クリアテロップを出していなかったら
		if (m_clear == nullptr)
		{
			//クリアテロップインスタンスの生成
			m_clear = NewGO<ClearTelop>(0);
		}
		//クリア後の経過時間が一定以上になれば
		if (m_colapsedTimeAfterClear >= FADE_START_VOLUME)
		{
			//フェードを開始する
			if (m_fade == nullptr)
			{
				m_fade = NewGO<Fade>(0);
			}
			//フェードの段階が待機状態ならば
			if (m_fade->GetFadePhase() == enWait)
			{
				//フェードアウトする命令を出す
				m_fade->SetFadeOut();
				//タイトル画面を作成
				Title* title = NewGO<Title>(0);
				//プレイしたゲームシーンを削除
				DeleteGO(this);
			}
		}
	}

	//ゲームオーバーシーンならば
	if (GameDirector::GetInstance().GetGameScene() == enGameOver)
	{
		//BGM音量を下げていく
		m_bgmVolume -= BGM_DECREASE_VOLUME;
		//0未満にならないようにする
		if (m_bgmVolume < 0.0f)
		{
			m_bgmVolume = 0.0f;
		}
		//BGM音量を設定する
		m_bgm->SetVolume(m_bgmVolume);
	}
	//ゲームオーバー演出が終わっていたら
	if(GameDirector::GetInstance().GetGameScene() == enAfterGameOver)
	{
		//遷移誘導インスタンスを作成する
		m_afterGOScene = NewGO<AfterGameOverScene>(0);
	}
}