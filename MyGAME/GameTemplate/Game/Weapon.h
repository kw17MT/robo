#pragma once
class Weapon : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;
public:
	virtual void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos);
};