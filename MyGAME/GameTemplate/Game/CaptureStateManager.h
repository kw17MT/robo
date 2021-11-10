#pragma once
#include "Enemy.h"

enum EnEnemyState
{
	None = 0,
	Captured,
	Targeted,
	ChangeMainTarget
};

//enum EnMissileTargetState
//{
//	enNoTarget,
//	enMissileTarget,
//	enDeleteIcon
//};

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

	//捕捉された敵の座標保存
	void SetCapturedEnemyPos(Vector3 enemyPos)
	{
		m_capturedEnemyPos = enemyPos;
	}
	//次の敵に自動的にロックオンを移すかどうか
	void SetNextEnemy(bool state)
	{
		m_captureNextEnemy = state;
	}
	//次にロックオンを移す敵の座標を設定
	void SetNextEnemyPos(Vector3 enemyPos)
	{
		m_capturedNextEnemyPos = enemyPos;
	}
	//次のロックオン対象先の座標とフラグをリセットする
	void ResetNextEnemyParam()
	{
		m_captureNextEnemy = false;
		m_capturedNextEnemyPos = Vector3::Zero;
	}

	//ロケットのターゲットをするかどうかを設定する
	void SetRocketTargetState(bool state)
	{
		//何もターゲットしてなく、新しくターゲットする場合
		if (m_rocketTargetedNum == 0)
		{
			//保存するターゲット座標のリセットを終える
			m_isResetPosNumber = false;
		}

		//ターゲットしたい
		if (state)
		{
			//けど、最大数に達していたなら
			if (m_rocketTargetedNum >= 9) 
			{
				//ターゲットしない
				m_isRocketTargetState = false;
			}
			else
			{
				m_isRocketTargetState = state;
			}
		}
		else
		{
			m_isRocketTargetState = state;
		}
	}

	void PlusRockeTargetNum()
	{
		//ターゲットする数をインクリメント
		m_rocketTargetedNum++; 
		m_isGetRocketTarget = true;
		//最大数以上に変数をインクリメントしたくないため
		if (m_rocketTargetedNum == 10)
		{
			m_isGetRocketTarget = false;
			m_rocketTargetedNum = 9;
		}
	}
	void MinusRockeTargetNum()
	{
		//ターゲットする数をインクリメント
		m_rocketTargetedNum--;
		//最大数以上に変数をインクリメントしたくないため
		if (m_rocketTargetedNum  <= 0)
		{
			m_rocketTargetedNum = 0;
		}
	}

	void SetRocketTargetPos(Vector3 pos , int num)
	{
		m_rocketTargets[num] = pos;		
	}

	void SetRocketTargetedEnemy(Enemy* enemy)
	{
		m_rocketTargetedEnemy[m_rocketTargetedNum] = enemy;
	}

	void ResetRocketTargetParam()
	{
		for (int i = 1; i < m_rocketTargetedNum; i++)
		{
			Vector3 resetPos = { 0.0f,0.0f,0.0f };
			m_rocketTargets[m_rocketTargetedNum - 1] = resetPos;
		}
		m_rocketTargetedNum = 0;
		m_isResetPosNumber = true;
	}
	void SetIsRocketTargeted(bool state)
	{
		m_isGetRocketTarget = state;
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
	bool GetRocketTargetState()
	{
		return m_isRocketTargetState;
	}
	int GetRocketTargetNum()
	{
		return m_rocketTargetedNum;
	}
	bool GetResetPosNumber()
	{
		return m_isResetPosNumber;
	}
	bool GetIsRocketTargeted()
	{
		return m_isGetRocketTarget;
	}
	Vector3 GetRocketTargetEnemyPos(int number)
	{
		return m_rocketTargets[number];
	}

	Enemy* GetRocketTargetEnemy(int number)
	{
		return m_rocketTargetedEnemy[number];
	}

private:
	EnEnemyState m_enemyState = None;
	Vector3 m_capturedEnemyPos = Vector3::Zero;

	bool m_captureNextEnemy = false;
	Vector3 m_capturedNextEnemyPos = Vector3::Zero;

	bool m_isRocketTargetState = false;
	int m_rocketTargetedNum = 0;
	std::array<Vector3, 10> m_rocketTargets;
	std::array<Enemy*, 10> m_rocketTargetedEnemy;

	bool m_isResetPosNumber = false;
	bool m_isGetRocketTarget = false;
};

