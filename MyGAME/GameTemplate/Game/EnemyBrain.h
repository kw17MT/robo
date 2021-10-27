#pragma once
#include "EnemyAtack.h"

enum EnEnemyBehaviourState
{
	enMove,
	enShoot,
	enRun
};

class EnemyBrain : public IGameObject
{
private:
	Vector3 m_playerPos = Vector3::Zero;
	Vector3 m_enemyPos = Vector3::Zero;
	int counter = 0;
	bool m_canShoot = false;
public:
	void MemoryPlayerPos(Vector3 playerPos) { m_playerPos = playerPos; }
	void MemoryEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }
	void Update();
private:
	EnemyAtack m_enemyAtack;			//çUåÇÇ∑ÇÈÇ©Ç«Ç§Ç©ÇÃîªífñÏ
};

