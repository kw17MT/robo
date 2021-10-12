#pragma once
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


	void SetOtherCapturedState(bool capturedState, Vector3 enemyPos)
	{
		m_someoneCaptured = capturedState;
		m_capturedEnemyPos = enemyPos;
	}
	bool GetOtherCapturedState()
	{
		return m_someoneCaptured;
	}
	Vector3 GetCapturedEnemyPos()
	{
		return m_capturedEnemyPos;
	}
private:
	bool m_someoneCaptured = false;
	Vector3 m_capturedEnemyPos = Vector3::Zero;
};

