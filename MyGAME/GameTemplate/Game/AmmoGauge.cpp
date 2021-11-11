#include "stdafx.h"
#include "AmmoGauge.h"
#include "SpriteRender.h"
#include "FontRender.h"

namespace
{
	const float BULLET_RELOAD_COMPLETE_TIME = 3.0f;
	const float MISSILE_RELOAD_COMPLETE_TIME = 5.0f;
	const float RASER_RELOAD_COMPLETE_TIME = 10.0f;
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

	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"0");

	return true;
}

void AmmoGauge::Reloading(const float reloadTime)
{
	m_scale.x = m_reloadTimer / reloadTime;
	if (m_reloadTimer >= reloadTime)
	{
		m_remaining_ammo = m_max_ammo;
		m_reloadTimer = 0.0f;
		m_isReloaded = true;
		m_finishReloading = true;
		return;
	}
	m_remaining_ammo = m_max_ammo * m_scale.x;
}

void AmmoGauge::Update()
{
	std::wstring distanceStr = std::to_wstring(m_remaining_ammo);
	m_fontRender->SetText(distanceStr.c_str());

	if (m_remaining_ammo == 0)
	{
		m_isReloaded = false;
	}

	if (!m_isReloaded)
	{
		m_reloadTimer += GameTime().GetFrameDeltaTime();

		switch (m_ammoType)
		{
		case enAmmoBullet:
			Reloading(BULLET_RELOAD_COMPLETE_TIME);
			break;
		case enAmmoMissile:
			Reloading(MISSILE_RELOAD_COMPLETE_TIME);
			break;
		case enAmmoRaser:
			Reloading(RASER_RELOAD_COMPLETE_TIME);
			break;
		}
	}
	else
	{
		m_finishReloading = false;
		m_scale.x = (float)m_remaining_ammo / (float)m_max_ammo;
	}

	m_fontRender->SetPosition({ -m_screenPos.x - 50.0f, m_screenPos.y + 20.0f });
	m_fontRender->SetScale(m_fontScale);
	m_spriteRender->SetScale(m_scale);
	m_spriteRender->SetPosition(m_screenPos);
}