#include "stdafx.h"
#include "CrossIcon.h"
#include "SpriteRender.h"

CrossIcon::~CrossIcon()
{
	DeleteGO(m_spriteRender);
}

bool CrossIcon::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticleNear.dds", 16, 16);

	return true;
}

void CrossIcon::Update()
{
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition(m_position);
}