#pragma once
class RoboMove
{
private:	
	Vector3 m_prevSpeed = Vector3::Zero;

public:
	const Vector3 Move(const Vector3 currentPos);
	Vector3 Inertia(Vector3 currentPos);
	Vector3 Execute(Vector3 currentPos);
};