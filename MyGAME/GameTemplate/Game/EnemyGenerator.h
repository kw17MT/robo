#pragma once
class Enemy;

class EnemyGenerator : public IGameObject
{
private:
	Vector3 m_firstEnemyPos = Vector3::Zero;
	int m_aliveEnemyNum = 0;
public:
	EnemyGenerator() {};
	~EnemyGenerator();
	bool Start();
	void Update();

	int GetEnemyNum() { return m_enemys.size(); }
	Vector3 GetEnemyPos(int no);
private:
	std::vector<Enemy*> m_enemys = { nullptr };
};

