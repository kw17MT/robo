#include "stdafx.h"
#include "RestrictArea.h"
#include "SpriteRender.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

RestrictArea::~RestrictArea()
{
	DeleteGO(m_spriteRender);
}

bool RestrictArea::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/warning/AreaWarn1.dds", 250, 150, "PSWarningSign");
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition({ 0.0f,200.0f,0.0f });

	return true;
}

void RestrictArea::JudgeInArea(Vector3 playerPos)
{
	float distance = CalcMethods::CalcDistance({ 0.0f,0.0f,0.0f }, playerPos);

	if (distance <= 50000.0f)
	{
		m_areaType = enSafeArea;
		m_scale = Vector3::Zero;
	}
	else if (distance <= 70000.0f)
	{
		m_areaType = enSemiDangerArea;
		m_scale = Vector3::One;
	}
	else
	{
		m_areaType = enDanger;
		DeleteGO(this);
	}
}

void RestrictArea::Update()
{
	m_spriteRender->SetScale(m_scale);
	if (m_scale.x == 1.0f)
	{
		if (m_addAlpha)
		{
			alpha += GameTime().GetFrameDeltaTime() / 2.0f;
		}
		else
		{
			alpha -= GameTime().GetFrameDeltaTime() / 2.0f;
		}

		if (alpha >= 1.0f)
		{
			m_addAlpha = false;
		}
		else if (alpha <= 0.0f)
		{
			m_addAlpha = true;
		}

		m_spriteRender->SetSpriteAlpha(alpha);
	}
}