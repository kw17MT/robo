#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"

namespace
{
	const float BULLET_SPEED = 200.0f;
}

bool MachinGun::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);

	return true;
}

Vector3 MachinGun::CalcToTargetVec()
{
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void MachinGun::Update()
{
	if (m_firstCalc == false)
	{
		m_moveSpeed = CalcToTargetVec() * BULLET_SPEED;
		m_firstCalc = true;
	}

	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);

	count += GameTime().GetFrameDeltaTime();
	if (count >= 10.0f)
	{
		DeleteGO(m_skinModelRender);
		DeleteGO(this);
	}
}