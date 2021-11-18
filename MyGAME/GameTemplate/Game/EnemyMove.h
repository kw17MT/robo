#pragma once

enum EnEnemyMoveTypes
{
	enApproach,					//プレイヤーにじわじわ接近する
	enFrontAndBehind,			//接近後、前後に移動する
	enPlayerBehind,				//プレイヤーの後ろを取るように通過と前後を繰り返す
	enAround,					//プレイヤーの周りを接近後ダッシュでいったん遠くに
	enApproachAndDash,			//接近後、プレイヤーを避けるように前方にダッシュ
	enStay,						//接近後、その場で滞留し、一定期間後ダッシュで他の方向に
	enDash
};

enum EnEnemyAltitudeState
{
	enTooHigh,
	enTooLow,
	enSafe
};

class EnemyMove
{
private:
	float m_moveSpeed = 0.0f;
	bool m_isTop = false;
	EnEnemyMoveTypes m_moveType = enApproach;
	EnEnemyMoveTypes m_prevMoveType = enApproach;
	EnEnemyAltitudeState m_altitudeState = enSafe;
	float m_adjustingAltitudeElapsedTime = 0.0f;
	float m_adjustedAltitudeHeight = 0.0f;				//高度調整用
	Vector3 m_prevMoveDirection = Vector3::Zero;


	bool m_moveForward = true;							//前後の動きの中で前進するかどうか
	bool m_isCalcEnemyDashDirection = false;							//敵はダッシュしたいるか
	float m_dashTimer = 0.0f;
	float m_aroundTimer = 0.0f;
	float m_waitTimer = 0.0f;

	float m_acceralation = 1.0f;

	Vector3 m_currentMoveDirection = Vector3::Zero;

public:

	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	 * @brief 現在の敵の移動方法を設定する
	 * @param type 移動方法
	*/
	void SetEnemyMoveTypes(EnEnemyMoveTypes type) { m_moveType = type; }

	/**
	 * @brief 現在の敵の高度の状態を設定する
	 * @param state 高度の程度
	*/
	void SetEnemyAltitudeState(EnEnemyAltitudeState state) { m_altitudeState = state; }

	/**
	* @brief 現在の敵の移動方法を取得する
	* @param type 移動方法
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief 現在の敵の高度の状態を取得する
	 * @return 高度の程度
	*/
	EnEnemyAltitudeState GetEnemyAltitudeState() { return m_altitudeState; }

	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief プレイヤーに接近モードの敵の次の位置座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次にの位置座標
	*/
	void CalcApproachSpeed(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief 敵の標高に応じて正常な高さに戻す
	*/
	Vector3 AdjustedAltitudeSpeed(Vector3 calcedPos);

	/**
	 * @brief 移動モードによって次の座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次の位置座標
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);

	void CalcFrontAndBehindSpeed(Vector3 enemyPos, Vector3 targetPos);

	void CalcAroundSpeed(Vector3 enemyPos, Vector3 targetPos);

	void CalcApproachAndDashSpeed(Vector3 enemyPos, Vector3 targetPos);

	void EnemyDashSpeed(Vector3 enemyPos, Vector3 targetPos, float dashTime = 5.0f);

	void EnemyStaySpeed(Vector3 enemyPos, Vector3 targetPos);

	void JudgeMoveType(Vector3 enemyPos, Vector3 targetPos);
private:
};

