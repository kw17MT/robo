#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"
#include "Rader.h"
#include "LaunchPad.h"
#include "SoundSource.h"

#include "GameDirector.h"
#include "CaptureStateManager.h"

namespace f
{
	const float BGM_VOLUME = 0.5f;
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

	//フラグ管理インスタンス削除
	CaptureStateManager::DeleteInstance();
}

bool Game::Start()
{
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
	//敵生成器作成
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	//BGM作成
	/*m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/bgm1.wav", false);
	m_bgm->SetVolume(f::BGM_VOLUME);
	m_bgm->Play(true);*/

	//ステートマネージャーの作成
	CaptureStateManager::CreateInstance();

	return true;
}

void Game::Update()
{
	//レーダーにプレイヤーの位置を与える
	m_rader->SetPlayerPos(m_player->GetPosition());
	//レーダーに生成した敵の数
	m_rader->SetEnemyNum(m_enemyGenerator->GetEnemyNum());
	//敵の数分その位置座標を保存、更新
	for (int i = 0; i < m_enemyGenerator->GetEnemyNum(); i++)
	{
		m_rader->SaveEnemyPos(i, m_enemyGenerator->GetEnemyPos(i));
	}

	if (g_pad[0]->IsPress(enButtonSelect))
	{
		DeleteGO(this);
	}
}