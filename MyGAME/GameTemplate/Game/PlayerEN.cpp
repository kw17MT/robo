#include "stdafx.h"
#include "PlayerEN.h"
#include "SpriteRender.h"

namespace
{
	const float MAX_EN = 100.0f;
}

PlayerEN::~PlayerEN()
{
	DeleteGO(m_spriteRender);
}

bool PlayerEN::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", 128, 32);
	m_spriteRender->SetPivot(m_pivot);
	m_spriteRender->SetPosition(m_screenPos);

	return true;
}

void PlayerEN::Update()
{
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		if (m_isPlayerEnRemain == true) {
			m_playerEN -= 0.5f;
			if (m_playerEN <= 0.0f)
			{
				m_playerEN = 0.0f;
			}
		}
		else
		{
			m_playerEN += 0.5f;
			if (m_playerEN >= 100.0f)
			{
				m_playerEN = 100.0f;
			}
		}
	}
	else
	{
		m_playerEN += 0.5f;
		if (m_playerEN >= 100.0f)
		{
			m_playerEN = 100.0f;
		}
	}

	m_scale.x = m_playerEN / MAX_EN;

	if (m_playerEN <= 0.0f)
	{
		m_playerEN = 0.0f;
		m_isPlayerEnRemain = false;
	}
	if (m_playerEN >= 100.0f)
	{
		m_isPlayerEnRemain = true;
	}

	m_spriteRender->SetScale(m_scale);
}