#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"
#include "AmmoGauge.h"
#include "Enemy.h"

namespace
{
	const int WAIT_SECOND = 1.0f;
	const int MAX_AMMO = 10;
}

MissileGenerator::~MissileGenerator()
{
	for (auto i : m_missiles)
	{
		DeleteGO(m_missiles.back());
		m_missiles.pop_back();
	}
	m_missiles.clear();
}

void MissileGenerator::SaveTargetedEnemy(Enemy* enemy)
{
	if (m_canTargetMore) {
		m_lockOnTargetNum++;
		if (m_lockOnTargetNum >= 10)
		{
			m_canTargetMore = false;
		}

		m_enemy.push_back(enemy);
	}
}

bool MissileGenerator::Start()
{
	m_ammoGauge = NewGO<AmmoGauge>(0);
	m_ammoGauge->SetRemainingAmmo(m_remaining_missile);
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	m_ammoGauge->SetAmmoType(enAmmoMissile);
	m_ammoGauge->SetPosition({ -400.0f, -200.0f, 0.0f });

	return true;
}

void MissileGenerator::Update()
{
	m_deleteMissileIcon = false;
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		//発射の準備しているよ
		m_isPrepareLaunch = true;
	}
	//発射の準備ができていて、ボタンを離したら
	if (m_isPrepareLaunch && !g_pad[0]->IsPress(enButtonLB2))
	{
		//発射数分ロケットをだす
		for (int i = 0; i < m_lockOnTargetNum; i++)
		{
			m_missiles.push_back(NewGO<Missile>(0));
			m_missiles.back()->SetEnemy(m_enemy.back());
			m_missiles.back()->SetLaunchedPos(m_launchPos);

			m_enemy.pop_back();
		}

		m_enemy.clear();
		m_isPrepareLaunch = false;
		m_deleteMissileIcon = true;
		m_canTargetMore = true;
		m_lockOnTargetNum = 0;
		CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
	}
}