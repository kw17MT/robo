#include "stdafx.h"
#include "EnemyIconOnRader.h"
#include "SpriteRender.h"

namespace
{
	const int ICON_SIZE = 12;
}

EnemyIconOnRader::~EnemyIconOnRader()
{
	DeleteGO(m_spriteRender);
}

bool EnemyIconOnRader::Start()
{
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->Init("Assets/Image/rader/enemyIcon.dds", ICON_SIZE, ICON_SIZE);

	return true;
}

void EnemyIconOnRader::Update()
{
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition(m_position);
	m_spriteRender->SetRotation(m_rot);
}