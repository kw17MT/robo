#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"
#include "Rader.h"

#include "CaptureStateManager.h"

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
	m_player = NewGO<Player>(0,"player");
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	m_sky = NewGO<SkyCube>(0);
	m_ground = NewGO<Ground>(0);
	m_ui = NewGO<UI>(0);
	m_sun = NewGO<Sun>(0);
	m_rader = NewGO<Rader>(0, "rader");

	CaptureStateManager::CreateInstance();

	return true;
}

void Game::Update()
{
	m_rader->SetPlayerPos(m_player->GetPosition());
	m_rader->SetEnemyNum(m_enemyGenerator->GetEnemyNum());
	for (int i = 1; i < m_enemyGenerator->GetEnemyNum(); i++)
	{
		m_rader->SaveEnemyPos(i - 1, m_enemyGenerator->GetEnemyPos(i));
	}
}