#pragma once
class SkinModelRender;
class EnemyAtack
{
private:
	Vector3 m_enemyPos = Vector3::Zero;
	Vector3 m_playerPos = Vector3::Zero;
	int counter = 0;
	bool startAtack = false;
public:
	void SetEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }
	void SetPlayerPos(Vector3 playerPos) { m_playerPos = playerPos; }
	bool Execute();
private:
};

