#pragma once
#include "Enemy.h"

/**
 * @brief レティクルの捕捉、ターゲット状態
*/
enum EnReticleState
{
	None,
	Captured,
	Targeted,
	ChangeMainTarget
};

/**
 * @brief レティクルのミサイルターゲット状態
*/
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
	//シングルトンインスタンス
	static CaptureStateManager* instance;
public:
	/**
	 * @brief インスタンスの作成
	*/
	static void CreateInstance()
	{
		instance = new CaptureStateManager;
	}

	/**
	 * @brief インスタンスの取得
	*/
	static CaptureStateManager& GetInstance()
	{
		static CaptureStateManager instance;
		return instance;
	}

	/**
	 * @brief インスタンスの削除
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief プレイヤーのロックオン状態を変更。次の敵を自動的にロックオン
	 * @param state プレイヤーのロックオン状態
	 * @param actively プレイヤーがロックオンを自分で解除したかどうか
	*/
	void SetCaptureState(EnReticleState state, bool actively = true)
	{
		m_enemyState = state;
	}

	/**
	 * @brief 捕捉状態を取得する
	 * @return 捕捉状態
	*/
	EnReticleState GetCaptureState()
	{
		return m_enemyState;
	}

	/**
	 * @brief レティクルがミサイルでターゲットしているかどうか設定する
	 * @param state ターゲットしているかどうか
	*/
	void SetMissileTargetState(EnMissileTargetState state)
	{
		m_isMissileTargetState = state;
	}

	/**
	 * @brief ミサイルのターゲット状態を取得する
	 * @return ミサイルのターゲット状態
	*/
	EnMissileTargetState GetMissileTargetState()
	{
		return m_isMissileTargetState;
	}

	/**
	 * @brief 次のターゲットが設定する
	 * @param isDecided 次をターゲットしているかどうか
	*/
	void SetIsDecidedNextTarget(bool isDecided)
	{
		m_isDecidedNextTarget = isDecided;
	}

	/**
	 * @brief 次をターゲットしているかどうかを取得する
	*/
	bool GetIsDecidedNextTarget()
	{
		return m_isDecidedNextTarget;
	}

private:
	EnReticleState m_enemyState = None;								//レティクルのターゲット状態
	EnMissileTargetState m_isMissileTargetState = enNoTarget;			//ミサイルのターゲット状態
	bool m_isDecidedNextTarget = false;									//次の敵をターゲットしているかどうか
};	