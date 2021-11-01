#include "stdafx.h"
#include "Weapon.h"

void Weapon::SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos)
{
	m_position = pos;
	m_targetPos = targetPos;
}