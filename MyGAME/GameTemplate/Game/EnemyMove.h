#pragma once

enum EnEnemyMoveTypes
{
	enNormalMove,
	enApproach,
	enRun,
	enAroundPlayer
};

class EnemyMove
{
private:
	bool m_isTop = false;
	EnEnemyMoveTypes m_moveType = enNormalMove;
	const Vector3 m_homePos = { 0.0f, 0.0f, 0.0f };
	Vector3 m_prevMoveDirection = Vector3::Zero;
public:
	/**
	 * @brief 現在の敵の移動方法を設定する
	 * @param type 移動方法
	*/
	void SetEnemyMoveTypes(EnEnemyMoveTypes type) { m_moveType = type; }

	/**
	* @brief 現在の敵の移動方法を取得する
	* @param type 移動方法
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief ターゲットの位置を中心に回る移動モードの時の次の位置座標を計算する
	 * @param enemyPos 現在の位置座標
	 * @param targetPos 回る中心にする座標（何も指定しないときは原点を中心に回す）
	 * @return 次の位置座標
	*/
	Vector3 CalcNextPos(Vector3 enemyPos, Vector3 targetPos = { 0.0f,0.0f,0.0f });

	/**
	 * @brief プレイヤーに接近モードの敵の次の位置座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次にの位置座標
	*/
	Vector3 CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief プレイヤーから逃げるモードの敵の次の位置座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次の位置座標
	*/
	Vector3 CalcRunPos(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief 移動モードによって次の座標を計算する。
	 * @param pos 現在の位置座標
	 * @return 次の位置座標
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);
private:
};

