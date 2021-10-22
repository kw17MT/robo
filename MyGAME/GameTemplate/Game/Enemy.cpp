#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	if (m_player != nullptr) {
		DeleteGO(m_player);
	}
}

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");
	m_skinModelRender = NewGO<SkinModelRender>(0, "enemy");
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr/*"Assets/modelData/enemy/enemy.tks"*/, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});

	m_displayDistance = NewGO<DisplayDistance>(0);
	m_displayDistance->SetEnemyPos(m_position);

	m_enemyStateIcon = NewGO<EnemyStateIcon>(0);
	m_enemyStateIcon->SetEnemyPos(m_position);
	m_enemyStateIcon->JudgeState(m_distance);

	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	//プレイヤーと自分（敵）の距離を計測し、自分にもその情報を保存
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
	//自分についてくるレティクルに位置座標を与える
	m_enemyStateIcon->SetEnemyPos(m_position);
	//プレイヤーと自分の距離を与えてレティクルの状態を更新する。
	m_enemyStateIcon->JudgeState(m_distance);
	//位置を更新
	m_skinModelRender->SetPosition(m_position);
}