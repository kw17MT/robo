#pragma once
#include "Enemy.h"

enum EnEnemyState
{
	None,
	Captured,
	Targeted,
	ChangeMainTarget
};

enum EnMissileTargetState
{
	enNoTarget,
	enMissileTarget,
	enMissileTargeted,
	enFull
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
	}

	EnEnemyState GetCaptureState()
	{
		return m_enemyState;
	}

	//ロケットのターゲットをするかどうかを設定する
	void SetMissileTargetState(EnMissileTargetState state)
	{
		m_isMissileTargetState = state;
	}

	EnMissileTargetState GetMissileTargetState()
	{
		return m_isMissileTargetState;
	}

private:
	EnEnemyState m_enemyState = None;
	EnMissileTargetState m_isMissileTargetState = enNoTarget;
};