#include "stdafx.h"
#include "EnemyBrain.h"
#include "GameDirector.h"

void EnemyBrain::Work()
{
	//座標と移動モードから次の位置座標を計算する。
	m_enemyPos = m_enemyMove.Execute(m_enemyPos, m_playerPos);
	//移動方向に敵のモデルの正面が向くような回転を計算する。
	m_enemyRotation.SetMoveDirection(m_enemyMove.GetEnemyMoveDireciton(), m_playerPos - m_enemyPos);
	m_rot = m_enemyRotation.GetCalcedRotation();

	//ゲーム中だけ弾を出すようにする
	if (GameDirector::GetInstance().GetGameScene() == enInGame)
	{
		m_canShoot = m_enemyAtack.Execute();
	}
}