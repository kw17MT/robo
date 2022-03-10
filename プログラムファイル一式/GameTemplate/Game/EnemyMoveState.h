#pragma once

#include "EnemyStateParam.h"

/**
 * @brief 移動状態の基底クラス
*/
class EnemyMoveState 
{
public:
	/**
	 * @brief デストラクタ
	*/
	virtual ~EnemyMoveState() {}

	/**
	 * @brief パラメータを初期化する
	 * @param param 移動制御のパラメータ
	*/
	virtual void Init(EnemyStateParam &param) = 0;

	/**
	 * @brief ステートの更新処理
	 * @param param 移動制御のパラメータ
	 * @return 適用するステート
	*/
	virtual EnemyMoveState* Update(EnemyStateParam &param) = 0;
};