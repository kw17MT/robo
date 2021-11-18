#include "stdafx.h"
#include "EnemyRotation.h"

namespace
{
	const Vector3 DEFAULT_MODEL_FRONT = { 0.0f,0.0f,1.0f };
}

Quaternion EnemyRotation::GetCalcedRotation()
{
	m_rot.SetRotation(DEFAULT_MODEL_FRONT, m_enemyToPlayerVec);

	return m_rot;
}