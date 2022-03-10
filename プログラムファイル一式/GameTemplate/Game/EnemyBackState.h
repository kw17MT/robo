#pragma once
#include "EnemyMoveState.h"

class EnemyBackState : public EnemyMoveState
{
public:
	/**
	 * @brief インスタンスを取得する
	 * @return 接近状態
	*/
	static EnemyBackState* GetInstance()
	{
		static EnemyBackState instance;
		return &instance;
	}

	/**
	 * @brief パラメータを初期化する
	 * @param param 移動制御のパラメータ
	*/
	virtual void Init(EnemyStateParam &param);

	/**
	　* @brief ステートの更新処理
	　* @param param 移動制御のパラメータ
	　* @return 適用するステート
	*/
	virtual EnemyMoveState* Update(EnemyStateParam &param);
};