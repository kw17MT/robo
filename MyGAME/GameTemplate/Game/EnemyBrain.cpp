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

bool EnemyBrain::JudgeCanShoot()
{
	if (m_canShoot)
	{
		return true;
	}
	return false;
}

void EnemyBrain::Work()
{
	//�U�����邩�̔��f��Ɏ����ƃv���C���[�̍��W��^����
	m_enemyAtack.SetEnemyPos(m_enemyPos);
	m_enemyAtack.SetPlayerPos(m_playerPos);

	//���W�ƈړ����[�h���玟�̈ʒu���W���v�Z����B
	m_enemyPos = m_enemyMove.Execute(m_enemyPos, m_playerPos);
	m_enemyRotation.SetMoveDirection(m_enemyMove.GetEnemyMoveDireciton(), m_playerPos - m_enemyPos);
	m_rot = m_enemyRotation.GetCalcedRotation();

	m_canShoot = m_enemyAtack.Execute();
}