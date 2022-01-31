#include "stdafx.h"
#include "Fade.h"
#include "SpriteRender.h"

namespace
{
}

Fade::~Fade()
{
	DeleteGO(m_sprite);
}

void Fade::SetStopFadeInProgress(bool isStopInProgress, float alpha)
{
	m_stopFadeInProgress = isStopInProgress;
	m_stopAlpha = alpha;
}

bool Fade::Start()
{
	m_sprite = NewGO<SpriteRender>(10);
	m_sprite->Init("Assets/image/fade/black.dds", 1280, 720);
	m_sprite->SetAlpha(m_alpha);
	m_fadePhase = enFadeIn;

	return true;
}

void Fade::Update()
{
	switch (m_fadePhase)
	{
	case enFadeIn:
		m_alpha += GameTime().GetFrameDeltaTime();

		if (m_stopFadeInProgress)
		{
			if (m_alpha > m_stopAlpha)
			{
				m_alpha = m_stopAlpha;
			}
		}

		if (m_alpha > 1.0f + GameTime().GetFrameDeltaTime())
		{
			m_alpha = 1.2f;
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

	m_sprite->SetAlpha(m_alpha);

	if (m_fadePhase == enFadeOut && m_alpha <= 0.0f)
	{
		DeleteGO(this);
	}
}