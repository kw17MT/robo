#pragma once

enum EnEnemyMoveTypes
{
	enApproach = -1,			//プレイヤーにじわじわ接近する
	enFrontAndBehind,			//接近後、前後に移動する
	enPlayerBehind,				//プレイヤーの後ろを取るように通過と前後を繰り返す
	enAround,					//プレイヤーの周りを接近後ダッシュでいったん遠くに
	enApproachAndDash,			//接近後、プレイヤーを避けるように前方にダッシュ
	enStay,						//接近後、その場で滞留し、一定期間後ダッシュで他の方向に
	enDash						//プレイヤーとすれ違うようにダッシュする。
};

class EnemyMove
{
private:
	EnEnemyMoveTypes m_moveType = enApproach;			//現在の移動タイプ
	EnEnemyMoveTypes m_prevMoveType = enApproach;		//前までの移動タイプ

	bool m_moveForward = true;							//前後の動きの中で前進するかどうか
	bool m_isCalcEnemyDashDirection = false;			//敵はダッシュしたいるか

	float m_dashTimer = 0.0f;							//ダッシュしている時間
	float m_aroundTimer = 0.0f;							//プレイヤー中心に旋回している時間
	float m_waitTimer = 0.0f;							//その場でプレイヤーを注視している時間
	float m_moveSpeed = 0.0f;							//その敵の移動速度
	float m_acceralation = 1.0f;						//加速度

	Vector3 m_currentMoveDirection = Vector3::Zero;		//現在の移動方向
	Vector3 m_dashDirection = Vector3::Zero;			//ダッシュの方向
public:
	/**
	 * @brief 敵の移動速度を設定
	 * @param moveSpeed	ユニークな移動速度
	*/
	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	* @brief 現在の敵の移動方法を取得する
	* @param type 移動方法
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief 敵の現在の移動方向を取得する
	 * @return 敵の移動方向
	*/
	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief プレイヤーに接近モードの敵の次の位置座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次にの位置座標
	*/
	void CalcApproachSpeed(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief 移動モードによって次の座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次の位置座標
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief プレイヤーに向かって前後運動するときの移動方向を計算する。
	 * @param toTargetVec プレイヤーへの方向ベクトル
	 * @param distanceBetweenEnemyToPlayer 敵とプレイヤーの距離
	*/
	void CalcFrontAndBehindDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief 旋回するときの移動方向を計算する。
	 * @param toTargetVec プレイヤーへの方向ベクトル
	*/
	void CalcAroundDirection(Vector3 toTargetVec);

	/**
	 * @brief 接近してダッシュでプレイヤーを通り過ぎる移動方向を計算する。
	 * @param toTargetVec プレイヤーへの方向ベクトル
	 * @param distanceBetweenEnemyToPlayer 敵とプレイヤーの距離
	*/
	void CalcApproachAndDashDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief ダッシュ方向を計算する。
	 * @param toTargetVec プレイヤーへの方向ベクトル
	*/
	void EnemyDashDirection(Vector3 toTargetVec);

	/**
	 * @brief 敵がプレイヤーの周りを漂うときの位置座標の更新
	 * @param toTargetVec プレイヤーへの方向ベクトル
	*/
	void EnemyStayDirection(Vector3 toTargetVec);

	/**
	 * @brief 敵とプレイヤーの位置関係によってどのように移動するか決める
	 * @param distanceBetweenEnemyToPlayer 敵とプレイヤーの距離
	*/
	void JudgeMoveType(float distanceBetweenEnemyToPlayer);
};

