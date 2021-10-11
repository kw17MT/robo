#include "stdafx.h"
#include "RoboMove.h"

namespace 
{
const float MOVE_SPEED = 1000.0f;
const float DECAY_RATE = 1.1f;
}

const Vector3 RoboMove::Move(const Vector3 currentPos)
{
	Vector3 nextPos = currentPos;
	Vector3 moveAmount = Vector3::Zero;
	Vector3 forwardDirection = g_camera3D->GetForward();
	Vector3 sideDirection = g_camera3D->GetRight();
	Vector3 upDirection = Cross(forwardDirection, sideDirection);

	moveAmount += sideDirection * MOVE_SPEED * g_pad[0]->GetLStickXF();
	moveAmount += forwardDirection * MOVE_SPEED * g_pad[0]->GetLStickYF();
	moveAmount += upDirection * MOVE_SPEED * g_pad[0]->IsPress(enButtonY);
	moveAmount -= upDirection * MOVE_SPEED * g_pad[0]->IsPress(enButtonA);

	m_prevSpeed = moveAmount;
	nextPos += moveAmount;

	return nextPos;
}

Vector3 RoboMove::Inertia(Vector3 currentPos)
{

	Vector3 nextPos = currentPos;
	m_prevSpeed /= DECAY_RATE;
	nextPos += m_prevSpeed;
	return nextPos;
}

Vector3 RoboMove::Execute(Vector3 currentPos)
{
	if (g_pad[0]->GetLStickXF()
		|| g_pad[0]->GetLStickYF()
		|| g_pad[0]->IsPress(enButtonA)
		|| g_pad[0]->IsPress(enButtonY))
	{
		return Move(currentPos);
	}

	return Inertia(currentPos);
}