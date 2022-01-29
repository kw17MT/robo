#include "stdafx.h"
#include "RestrictArea.h"
#include "SpriteRender.h"
#include "effect/Effect.h"
#include "SoundSource.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float SE_VOLUME = 1.0f;
	const float FAR_DISTANCE = 70000.0f;
}

RestrictArea::~RestrictArea()
{
	DeleteGO(m_spriteRender);
}

bool RestrictArea::Start()
{
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->Init("Assets/Image/warning/AreaWarn.dds", 250, 150, "PSWarningSign");
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition({ 0.0f,200.0f,0.0f });

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/areaResNear.efk");
	m_effect->SetPosition(Vector3::Zero);
	m_effect->SetScale({ 8000.0f,5000.0f,8000.0f });
	m_effect->Update();

	m = NewGO<Effect>(0);
	m->Init(u"Assets/effect/areaResFar.efk");
	m->SetPosition(Vector3::Zero);
	m->SetScale({ 11500.0f,5000.0f,11500.0f });
	m->Update();

	return true;
}

void RestrictArea::JudgeInArea(Vector3 playerPos)
{
	Vector3 posXZ = playerPos;
	posXZ.y = 0.0f;
	m_distance = CalcMethods::CalcDistance({ 0.0f,0.0f,0.0f }, posXZ);

	if (m_distance <= 50000.0f)
	{
		m_areaType = enSafeArea;
		m_scale = Vector3::Zero;
	}
	else if (m_distance <= 70000.0f)
	{
		m_areaType = enSemiDangerArea;
		m_scale = Vector3::One;
	}
	else
	{
		m_areaType = enDanger;
		DeleteGO(this);
	}

	if (m_distance >= 42000.0f)
	{
		m_areaRestrictTimer += GameTime().GetFrameDeltaTime();

		if (m_areaRestrictTimer >= 1.0f)
		{
			m_effect->Play(false);
			m->Play(false);
			m_areaRestrictTimer = 0.0f;
		}
	}
}

float RestrictArea::GetMonochromeRateDependOnDistance()
{
	float monochromeRate = m_distance / FAR_DISTANCE;
	return 	monochromeRate;
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