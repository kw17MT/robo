#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"

bool MachinGun::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);

	return true;
}

Vector3 MachinGun::CalcToTargetVec()
{
	Vector3 toTargetVec = m_targetPos - m_position;
	return toTargetVec /= 100.0f;
}

void MachinGun::Update()
{
	m_position += CalcToTargetVec();
	m_skinModelRender->SetPosition(m_position);

	count += GameTime().GetFrameDeltaTime();
	if (count >= 10.0f)
	{
		DeleteGO(m_skinModelRender);
		DeleteGO(this);
	}
}