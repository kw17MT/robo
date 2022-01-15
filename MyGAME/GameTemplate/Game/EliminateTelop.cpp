#include "stdafx.h"
#include "EliminateTelop.h"
#include "SpriteRender.h"
#include "SoundSource.h"

EliminateTelop::~EliminateTelop()
{
	for (int i = 0; i < 2; i++)
	{
		DeleteGO(m_sprite[i]);
	}
}

bool EliminateTelop::Start()
{
	for (int i = 0; i < 2; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(10);
		m_sprite[i]->SetPosition(m_position);
		m_sprite[i]->SetScale(m_scale);
	}
	m_sprite[0]->Init("Assets/Image/Eliminate/eliminate_frame.dds", 2000, 300);
	m_sprite[1]->Init("Assets/Image/Eliminate/eliminate.dds", 1000, 500);

	return true;
}

void EliminateTelop::Update()
{
	m_elapsedTime += GameTime().GetFrameDeltaTime();

	if (m_elapsedTime >= 1.0f
		&& !m_shouldSlideInFrame)
	{
		m_shouldSlideInFrame = true;

		CSoundSource* inSE = NewGO<CSoundSource>(0);
		inSE->Init(L"Assets/sound/Eliminate_In.wav");
		inSE->SetVolume(0.5f);
		inSE->Play(false);

	}
	else if (m_elapsedTime >= 6.0f
		&& !m_shouldDeleteTelop)
	{
		m_shouldDeleteTelop = true;

		CSoundSource* inSE = NewGO<CSoundSource>(0);
		inSE->Init(L"Assets/sound/Eliminate_Out.wav");
		inSE->SetVolume(0.35f);
		inSE->Play(false);
	}

	if (m_shouldSlideInFrame)
	{
		if (!m_shouldDeleteTelop)
		{
			m_position.x -= 60.0f;
			if (m_position.x < 0.0f)
			{
				m_position.x = 0.0f;
			}
		}
		else
		{
			m_position.x -= 120.0f;
			if (m_position.x <= -2000.0f)
			{
				m_shouldDestroyInstance = true;
			}
		}
		m_sprite[0]->SetPosition(m_position);
		m_sprite[1]->SetPosition(m_position);
	}

	if (m_shouldDeleteTelop)
	{
		
	}
}