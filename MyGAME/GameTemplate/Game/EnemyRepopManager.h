#pragma once
class EnemyRepopManager
{
private:
	static EnemyRepopManager* instance;
	bool m_shouldRepopEnemy = false;
public:
	static void CreateInstance()
	{
		instance = new EnemyRepopManager;
	}
	static EnemyRepopManager& GetInstance()
	{
		static EnemyRepopManager instance;
		return instance;
	}
	static void DeleteInstance()
	{
		delete instance;
	}

	void SetRepopEnemy(bool state)
	{
		m_shouldRepopEnemy = state;
	}
};

