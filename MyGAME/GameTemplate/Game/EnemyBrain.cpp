#include "stdafx.h"
#include "EnemyBrain.h"

#include "Calculation.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float RECOGNITION_DISTANCE = 20000.0f;
	const float LOWEST_ALTITUDE = -10000.0f;
	const float HIGHEST_ALTITUDE = 30000.0f;
}

void EnemyBrain::JudgeMoveType()
{
	//プレイヤーと自分の距離を計算する。
	float distance = CalcMethods::CalcDistance(m_enemyPos, m_playerPos);
	//敵の移動モードがなにもターゲットしていない状態で距離が一定以下ならば
	if (distance < RECOGNITION_DISTANCE && m_enemyMove.GetEnemyMoveTypes() == enNormalMove)
	{
		//接近モードにする。
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}
	//敵の移動モードが逃げるモードかプレイヤーを囲むモードで距離が一定以上ならば
	else if((m_enemyMove.GetEnemyMoveTypes() == enSurroundPlayer 
		|| m_enemyMove.GetEnemyMoveTypes() == enRun)
		&& distance > RECOGNITION_DISTANCE)
	{
		//プレイヤーの周りをぐるぐる回るモードにする。
		m_enemyMove.SetEnemyMoveTypes(enAroundPlayer);
	}
	//回るモードで一定距離以下になれば
	else if (m_enemyMove.GetEnemyMoveTypes() == enAroundPlayer && distance < RECOGNITION_DISTANCE)
	{
		//再度接近モードにする。
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}

	//最低高度まで敵が逃げてきたら
	if (m_enemyPos.y <= LOWEST_ALTITUDE && m_enemyMove.GetEnemyAltitudeState() == enRun)
	{
		//高度調整モードにはいる
		m_enemyMove.SetEnemyAltitudeState(enTooLow);
	}
	//最高硬度まで敵が逃げてきたら
	else if (m_enemyPos.y >= HIGHEST_ALTITUDE && m_enemyMove.GetEnemyAltitudeState() == enRun)
	{
		//高度調整モードに入る
		m_enemyMove.SetEnemyAltitudeState(enTooHigh);
	}
}

void EnemyBrain::Work()
{
	//攻撃するかの判断野に自分とプレイヤーの座標を与える
	m_enemyAtack.SetEnemyPos(m_enemyPos);
	m_enemyAtack.SetPlayerPos(m_playerPos);

	//座標と移動モードから次の位置座標を計算する。
	m_enemyPos = m_enemyMove.Execute(m_enemyPos, m_playerPos);
	JudgeMoveType();

	m_canShoot = m_enemyAtack.Execute();
}