#include "stdafx.h"
#include "MissileBallistic.h"

void MissileBallistic::DecideBallisticType()
{
	int type = rand() % enTypeNum;

	switch (type)
	{
	case enStraight:
		m_type = enStraight;
		break;
	case enMeandeling:
		m_type = enMeandeling;
		break;
	case enRolling:
		m_type = enRolling;
		break;
	}
}

void MissileBallistic::ProgressDegreeChange()
{
	if (m_return == false)
	{
		m_cosVol += 1.0f;

		if (m_cosVol >= 180.0f)
		{
			m_cosVol = 0.0f;
			//m_return = true;
		}
	}
	else if (m_return == true)
	{
		m_cosVol -= 0.05f;

		/*if (m_cosVol <= 1.0f)
		{
			m_cosVol = 1.0f;
			m_return = false;
		}*/
	}
}

Vector3 MissileBallistic::GetBallisticDirection()
{
	ProgressDegreeChange();
	float x = cos(m_cosVol);
	x *= 9.0f;
	float y = sin(m_sinVol);


	Vector3 dir = { x,y,0.0f };
	dir.Normalize();
	Quaternion a;
	a.SetRotation({ 0.0f,1.0f,0.0f }, m_cameraForward);
	a.Apply(dir);

	senkaiTime -= GameTime().GetFrameDeltaTime() / 5.0f;
	if (senkaiTime <= 0.0f)
	{
		senkaiTime = 0.0f;
	}

	return dir * senkaiTime;
}