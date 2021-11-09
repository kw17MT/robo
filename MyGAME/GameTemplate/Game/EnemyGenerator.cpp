#include "stdafx.h"
#include "EnemyGenerator.h"
#include "Enemy.h"

namespace
{
	const int MAX_ENEMY_NUM = 2;
	const float ENEMY_SPACE = 1000.0f;
}

EnemyGenerator::~EnemyGenerator()
{
	for (auto i  : m_enemys)
	{
		DeleteGO(m_enemys.back());
		m_enemys.pop_back();
	}
	m_enemys.clear();
}

bool EnemyGenerator::Start()
{
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		m_enemys.push_back(NewGO<Enemy>(0, "enemy"));
		Vector3 enemyPos = m_firstEnemyPos;
		enemyPos.x += ENEMY_SPACE * (i + 1);
		m_enemys.back()->SetPosition(enemyPos);
		m_aliveEnemyNum++;
	}

	return true;
}

void EnemyGenerator::Update()
{

}