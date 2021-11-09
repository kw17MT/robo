#include "stdafx.h"
#include "EnemyBrain.h"

#include "Calculation.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

void EnemyBrain::JudgeMoveType()
{
	//プレイヤーと自分の距離を計算する。
	float distance = CalcMethods::CalcDistance(m_enemyPos, m_playerPos);
	//敵の移動モードがなにもターゲットしていない状態で距離が一定以下ならば
	if (distance < 20000.0f && m_enemyMove.GetEnemyMoveTypes() == enNormalMove)
	{
		//接近モードにする。
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}
	//敵の移動モードが逃げる状態で距離が一定以上ならば
	else if(m_enemyMove.GetEnemyMoveTypes() == enRun && distance > 21000.0f)
	{
		//プレイヤーの周りをぐるぐる回るモードにする。
		m_enemyMove.SetEnemyMoveTypes(enAroundPlayer);
	}
	//回るモードで一定距離以下になれば
	else if (m_enemyMove.GetEnemyMoveTypes() == enAroundPlayer && distance < 20000.0f)
	{
		//再度接近モードにする。
		m_enemyMove.SetEnemyMoveTypes(enApproach);
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