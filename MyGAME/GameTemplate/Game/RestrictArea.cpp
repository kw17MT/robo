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
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", 128, 32);
	m_spriteRender->SetScale(m_scale);

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
}