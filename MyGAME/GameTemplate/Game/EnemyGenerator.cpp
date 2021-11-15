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
	//生成した敵の削除
	for (auto i  : m_enemys)
	{
		DeleteGO(m_enemys.back());
		m_enemys.pop_back();
	}
	m_enemys.clear();
}


bool EnemyGenerator::Start()
{
	//敵を最大数作成
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		m_enemys.push_back(NewGO<Enemy>(0, "enemy"));
		Vector3 enemyPos = m_firstEnemyPos;
		//生成された番号で位置をずらす
		enemyPos.x += ENEMY_SPACE * (i + 1);
		m_enemys.back()->SetPosition(enemyPos);
	}

	return true;
}

Vector3 EnemyGenerator::GetEnemyPos(int no)
{
	return  m_enemys[no]->GetPosition(); 
}

void EnemyGenerator::Update()
{

}