#include "stdafx.h"
#include "Fade.h"
#include "SpriteRender.h"

namespace
{
	const Vector4 BLACK = { 0.0f,0.0f,0.0f, 0.0f };
}

Fade::~Fade()
{
	DeleteGO(m_sprite);
}

bool Fade::Start()
{
	m_sprite = NewGO<SpriteRender>(0);
	m_sprite->Init("Assets/image/fade/black.dds", 1280, 720);
	m_sprite->SetColor(BLACK);
	m_fadePhase = enFadeIn;

	return true;
}

void Fade::Update()
{
	switch (m_fadePhase)
	{
	case enFadeIn:
		m_alpha += GameTime().GetFrameDeltaTime();
		if (m_alpha > 1.0f)
		{
			m_alpha = 1.0f;
			m_fadePhase = enWait;
		}
		break;
	case enFadeOut:
		m_alpha -= GameTime().GetFrameDeltaTime();
		if (m_alpha < 0.0f)
		{
			m_alpha = 0.0f;
		}
		break;
	case enWait:
		break;
	default:
		break;
	}

	Vector4 color = BLACK;
	color.w = m_alpha;
	m_sprite->SetColor(color);

	if (m_fadePhase == enFadeOut && m_alpha <= 0.0f)
	{
		DeleteGO(this);
	}
}