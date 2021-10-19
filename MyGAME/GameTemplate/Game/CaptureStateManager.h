#pragma once

enum EnEnemyState
{
	None = 0,
	Captured,
	Targeted
};

class CaptureStateManager
{
private:
	static CaptureStateManager* instance;
public:
	static void CreateInstance()
	{
		instance = new CaptureStateManager;
	}
	static CaptureStateManager& GetInstance()
	{
		static CaptureStateManager instance;
		return instance;
	}
	static CaptureStateManager DeleteInstance()
	{
		delete instance;
	}

	void SetCaptureState(EnEnemyState state)
	{
		m_enemyState = state;
	}
	void SetCapturedEnemyPos(Vector3 enemyPos)
	{
		m_capturedEnemyPos = enemyPos;
	}

	EnEnemyState GetCaptureState()
	{
		return m_enemyState;
	}

	Vector3 GetCapturedEnemyPos()
	{
		return m_capturedEnemyPos;
	}

private:
	EnEnemyState m_enemyState = None;
	Vector3 m_capturedEnemyPos = Vector3::Zero;
};

