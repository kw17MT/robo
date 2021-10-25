#pragma once

enum EnEnemyState
{
	None = 0,
	Captured,
	Targeted,
	ChangeMainTarget
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
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief プレイヤーのロックオン状態を変更。次の敵を自動的にロックオン
	 * @param state プレイヤーのロックオン状態
	 * @param actively プレイヤーがロックオンを自分で解除したかどうか
	*/
	void SetCaptureState(EnEnemyState state, bool actively = true)
	{
		m_enemyState = state;

		//状態遷移の先が何もロックオンしていない状態で、プレイヤーが能動的にNoneにしていなければ
		if (state == None && actively == false)
		{
			//次にターゲットする敵が決まっていたら
			if (m_captureNextEnemy)
			{
				//自動的にターゲットする位置座標をそこにする。
				m_capturedEnemyPos = m_capturedNextEnemyPos;
				//次の敵をロックオンしているため、状態をロックオンに
				m_enemyState = ChangeMainTarget;
				//次のロックオン先を再度決めたいためフラグと座標を元に戻す。
				m_captureNextEnemy = false;
				Vector3 homePos = { 0.0f,0.0f,-1.0f };
				m_capturedNextEnemyPos = homePos;
			}
			else
			{
				Vector3 homePos = { 0.0f,0.0f,-1.0f };
				m_capturedEnemyPos = homePos;
			}
		}
	}
	void SetCapturedEnemyPos(Vector3 enemyPos)
	{
		m_capturedEnemyPos = enemyPos;
	}
	void SetNextEnemy(bool state)
	{
		m_captureNextEnemy = state;
	}
	void SetNextEnemyPos(Vector3 enemyPos)
	{
		m_capturedNextEnemyPos = enemyPos;
	}

	EnEnemyState GetCaptureState()
	{
		return m_enemyState;
	}
	Vector3 GetCapturedEnemyPos()
	{
		return m_capturedEnemyPos;
	}
	bool IsNextEnemyCaptured()
	{
		return m_captureNextEnemy;
	}
	Vector3 GetCapturedNextEnemyPos()
	{
		return m_capturedNextEnemyPos;
	}

private:
	EnEnemyState m_enemyState = None;
	Vector3 m_capturedEnemyPos = Vector3::Zero;

	bool m_captureNextEnemy = false;
	Vector3 m_capturedNextEnemyPos = Vector3::Zero;
};

