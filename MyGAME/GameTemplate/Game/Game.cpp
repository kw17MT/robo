#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"

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


	CaptureStateManager::CreateInstance();

	return true;
}

void Game::Update()
{

}