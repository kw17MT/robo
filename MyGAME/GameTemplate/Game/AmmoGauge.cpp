#include "stdafx.h"
#include "AmmoGauge.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_RELOAD_COMPLETE_TIME = 3.0f;
	const int MISSILE_RELOAD_COMPLETE_TIME = 5.0f;
	const int RASER_RELOAD_COMPLETE_TIME = 10.0f;
}

AmmoGauge::~AmmoGauge()
{
	DeleteGO(m_spriteRender);
}

bool AmmoGauge::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", 128, 32);
	m_spriteRender->SetPivot(m_pivot);

	return true;
}

void AmmoGauge::Update()
{
	if (m_remaining_ammo == 0)
	{
		m_isReloaded = false;
	}

	if (!m_isReloaded)
	{
		m_ReloadTimer += GameTime().GetFrameDeltaTime();

		switch (m_ammoType)
		{
		case enBullet:
			m_scale.x = m_ReloadTimer / BULLET_RELOAD_COMPLETE_TIME;
			if (m_ReloadTimer >= BULLET_RELOAD_COMPLETE_TIME)
			{
				m_remaining_ammo = m_max_ammo;
				m_ReloadTimer = 0.0f;
				m_isReloaded = true;
				m_finisReloading = true;
				return;
			}
			break;
		case enMissile:
			m_scale.x = m_ReloadTimer / MISSILE_RELOAD_COMPLETE_TIME;
			if (m_ReloadTimer >= MISSILE_RELOAD_COMPLETE_TIME)
			{
				m_remaining_ammo = m_max_ammo;
				m_ReloadTimer = 0.0f;
				m_isReloaded = true;
				m_finisReloading = true;
				return;
			}
			break;
		case enRaser:
			m_scale.x = m_ReloadTimer / RASER_RELOAD_COMPLETE_TIME;
			if (m_ReloadTimer >= RASER_RELOAD_COMPLETE_TIME)
			{
				m_remaining_ammo = m_max_ammo;
				m_ReloadTimer = 0.0f;
				m_isReloaded = true;
				m_finisReloading = true;
				return;
			}
			break;
		}
	}
	else
	{
		m_finisReloading = false;
		m_scale.x = (float)m_remaining_ammo / (float)m_max_ammo;
	}

	m_spriteRender->SetScale(m_scale);
}