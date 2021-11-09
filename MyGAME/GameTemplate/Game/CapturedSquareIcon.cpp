#include "stdafx.h"
#include "CapturedSquareIcon.h"
#include "SpriteRender.h"

CapturedSquareIcon::~CapturedSquareIcon()
{
	DeleteGO(m_spriteRender);
}

bool CapturedSquareIcon::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticle_near2.dds", 48, 48);

	return true;
}

void CapturedSquareIcon::Update()
{
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition(m_position);
}