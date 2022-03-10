#pragma once
#include "EnemyApproachState.h"

class EnemyMove
{
private:
	bool m_isCalcEnemyDashDirection = false;			//敵はダッシュしたいるか
	float m_moveSpeed = 0.0f;							//その敵の移動速度
	float m_acceralation = 1.0f;						//加速度
	Vector3 m_currentMoveDirection = Vector3::Zero;		//現在の移動方向
	Vector3 m_dashDirection = Vector3::Zero;			//ダッシュの方向
	EnemyStateParam m_param;							//移動遷移に使用するパラメータ
	EnemyMoveState* m_state;							//現在の移動状態

public:
	/**
	 * @brief コンストラクタ、移動モードを接近で初期化
	*/
	EnemyMove() { m_state = EnemyApproachState::GetInstance();}

	/**
	 * @brief 敵の移動速度を設定
	 * @param moveSpeed	ユニークな移動速度
	*/
	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	 * @brief 敵の現在の移動方向を取得する
	 * @return 敵の移動方向
	*/
	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief 接近方向を計算する
	 * @param toTargetVec ターゲットへの方向
	*/
	void CalcApproachDirection(Vector3 toTargetVec);

	/**
	 * @brief 後退方向を計算する
	 * @param toTargetVec ターゲットへの方向
	*/
	void CalcBackDirection(Vector3 toTargetVec);

	/**
	* @brief 旋回方向を計算する
	* @param toTargetVec ターゲットへの方向
	*/
	void CalcAroundDirection(Vector3 toTargetVec);

	/**
	* @brief ダッシュ方向を計算する
	* @param toTargetVec ターゲットへの方向
	*/
	void CalcDashDirection(Vector3 toTargetVec);

	/**
	* @brief じっくり動く方向を計算する
	* @param toTargetVec ターゲットへの方向
	*/
	void CalcStayDirection(Vector3 toTargetVec);

	/**
	* @brief 移動モードによって次の座標を計算する。
	* @param enemypos 現在の位置座標
	* @param targetPos ターゲットとなるオブジェクトの位置座標
	* @return 次の位置座標
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);
};

