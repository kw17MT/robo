#include "stdafx.h"
#include "PlayerHP.h"
#include "SpriteRender.h"
#include "FontRender.h"

namespace
{
	const float BULLET_DAMAGE = 0.0f;
	const float MISSILE_DAMAGE = 10.0f;
	const float RASER_DAMAGE = 20.0f;
	const float MAX_HP = 100.0f;
}

PlayerHP::~PlayerHP()
{
	DeleteGO(m_spriteRender);
}

void PlayerHP::ApplyDamage(EnPlayerDamageTypes enemyDamageType)
{
	switch (enemyDamageType)
	{
	case enEnemyBullet:
		m_playerHp -= BULLET_DAMAGE;
		break;
	case enEnemyMissile:
		m_playerHp -= MISSILE_DAMAGE;
		break;
	case enEnemyRaser:
		m_playerHp -= RASER_DAMAGE;
		break;
	}

	
}

bool PlayerHP::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", 128, 32);
	m_spriteRender->SetPivot(m_pivot);
	m_spriteRender->SetPosition(m_screenPos);

	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"");
	m_fontRender->SetScale(0.5f);
	return true;
}

void PlayerHP::Update()
{
	std::wstring playerHp = std::to_wstring((int)m_playerHp);
	m_fontRender->SetText(playerHp.c_str());

	m_scale.x = m_playerHp / MAX_HP;

	if (m_playerHp <= 0.0f)
	{
		m_playerHp = 0;
		m_isPlayerAlive = false;
	}

	m_fontRender->SetPosition({ m_screenPos.x, m_screenPos.y });
	m_spriteRender->SetScale(m_scale);
}