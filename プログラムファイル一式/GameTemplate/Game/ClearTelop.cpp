#include "stdafx.h"
#include "ClearTelop.h"
#include "SpriteRender.h"

namespace
{
	const Vector3 SHRINK_RATE = { 0.01f,0.01f,0.01f };
}

ClearTelop::~ClearTelop()
{
	for (int i = 0; i < 3; i++)
	{
		DeleteGO(m_sprite[i]);
	}
}

bool ClearTelop::Start()
{
	for (int i = 0; i < 3; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(1);
	}
	m_sprite[0]->Init("Assets/image/Clear/clearSurround.dds", 500, 500);
	m_sprite[1]->Init("Assets/image/Clear/clearButtom.dds", 500, 500);
	m_sprite[2]->Init("Assets/image/Clear/missionClear.dds", 500, 500);

	return true;
}

void ClearTelop::Update()
{
	if (m_surroundScale.x > 0.8f)
	{
		m_surroundScale -= SHRINK_RATE;
		m_angle += 1.0f;
	}
	else
	{
		m_surroundScale = { 0.8f,0.8f,0.8f };
	}

	m_rot.SetRotationZ(m_angle);

	m_sprite[0]->SetScale(m_surroundScale);
	m_sprite[0]->SetRotation(m_rot);
}