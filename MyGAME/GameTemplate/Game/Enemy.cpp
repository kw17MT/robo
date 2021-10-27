#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"
#include "CaptureStateManager.h"
#include "EnemyBrain.h"
#include "MachinGun.h"

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	DeleteGO(m_enemyHP);

	//敵が倒れたため、ロックオン状態を何もなしにするか、次の敵をロックオンするか決定する。
	CaptureStateManager::GetInstance().SetCaptureState(None, false);
}

void Enemy::TakenDamage(EnDamageTypes damageType)
{
	m_enemyHP->ApplyDamage(damageType);
}

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr/*"Assets/modelData/enemy/enemy.tks"*/, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});

	m_displayDistance = NewGO<DisplayDistance>(0);
	m_displayDistance->SetEnemyPos(m_position);

	m_enemyStateIcon = NewGO<EnemyStateIcon>(0);
	m_enemyStateIcon->SetEnemyPos(m_position);
	m_enemyStateIcon->JudgeState(m_distance);

	m_enemyHP = NewGO<EnemyHP>(0);
	m_enemyHP->SetEnemyPos(m_position);
	m_enemyHP->IsEnemyTargeted(false);

	m_enemyBrain = NewGO<EnemyBrain>(0);
	m_enemyBrain->MemoryPlayerPos(m_player->GetPosition());
	m_enemyBrain->MemoryEnemyPos(m_position);

	//m_machinGun = NewGO<MachinGun>(0);

	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	m_position = m_enemyMove.CalcNextPos(m_position);
	//プレイヤーと自分（敵）の距離を計測し、自分にもその情報を保存
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
	//自分についてくるレティクルに位置座標を与える
	m_enemyStateIcon->SetEnemyPos(m_position);
	//プレイヤーと自分の距離を与えてレティクルの状態を更新する。
	m_enemyStateIcon->JudgeState(m_distance);
	//位置を更新
	m_skinModelRender->SetPosition(m_position);

	m_enemyHP->IsEnemyTargeted(m_enemyStateIcon->IsTargeted());
	//HPバーの位置を更新
	m_enemyHP->SetEnemyPos(m_position);

	m_enemyBrain->MemoryPlayerPos(m_player->GetPosition());
	m_enemyBrain->MemoryEnemyPos(m_position);


	//m_machinGun->SetTargetAndCurrentPos(m_player->GetPosition(), m_position);


	//HPがなくなったら
	if (m_enemyHP->IsDead())
	{
		//消す
		DeleteGO(this);
	}
}