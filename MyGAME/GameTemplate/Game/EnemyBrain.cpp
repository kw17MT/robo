#include "stdafx.h"
#include "EnemyBrain.h"

void EnemyBrain::Update()
{
	m_enemyAtack.SetEnemyPos(m_enemyPos);
	m_enemyAtack.SetPlayerPos(m_playerPos);

	m_canShoot = m_enemyAtack.Execute();
}