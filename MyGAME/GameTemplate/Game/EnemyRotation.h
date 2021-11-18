#pragma once

class EnemyRotation
{
private:
	Vector3 m_moveDirection = Vector3::Zero;
	Vector3 m_enemyToPlayerVec = Vector3::Zero;
	Quaternion m_rot;
public:
	void SetMoveDirection(Vector3 dir, Vector3 vec) { m_moveDirection = dir; m_enemyToPlayerVec = vec; }

	Quaternion GetCalcedRotation();
};