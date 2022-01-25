#pragma once
class EnemyRepopManager
{
private:
	static EnemyRepopManager* instance;
	bool m_shouldRepopEnemy = false;
	int m_popedNum = 0;
	int m_killedNum = 0;
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

	bool ShouldRepopEnemy()
	{
		if (m_popedNum != 0
			&& m_popedNum == m_killedNum)
		{
			m_popedNum = 0;
			m_killedNum = 0;
			return true;
		}
		return false;
	}

	void SetPopedEnemyNum(int num)
	{
		m_popedNum = num;
	}

	void AddKilledEnemyNum()
	{
		m_killedNum++;
	}

	void ResetParam()
	{
		m_shouldRepopEnemy = false;
		m_popedNum = 0;
		m_killedNum = 0;
	}
};

