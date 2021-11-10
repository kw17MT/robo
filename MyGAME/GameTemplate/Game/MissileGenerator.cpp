#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"
#include "AmmoGauge.h"

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
		//���˂̏������Ă����
		m_isPrepareLaunch = true;
	}
	//���˂̏������ł��Ă��āA�{�^���𗣂�����
	if (m_isPrepareLaunch && !g_pad[0]->IsPress(enButtonLB2))
	{
		//���ˊJ�n
		m_isLaunch = true;
	}
	//����
	if (m_isLaunch)
	{
		//���ː������P�b�g������
		m_launchNum = CaptureStateManager::GetInstance().GetRocketTargetNum();
		for (int i = 0; i < m_launchNum; i++)
		{
			m_missiles.push_back(NewGO<Missile>(0));
			//1��1���o��������ꏊ��ς���
			Vector3 individualPos = m_launchPos;

			m_missiles.back()->SetTargetAndCurrentPos(CaptureStateManager::GetInstance().GetRocketTargetEnemyPos(i), individualPos);
			m_missiles.back()->SetEnemy(CaptureStateManager::GetInstance().GetRocketTargetEnemy(i));
			m_missiles.back()->SetNumber(i);
		}
		//���P�b�g�𔭎˂������߁A���b�N�I�����Ă����G�̐��Əꏊ�����Z�b�g
		CaptureStateManager::GetInstance().ResetRocketTargetParam();

		m_isPrepareLaunch = false;
		m_isLaunch = false;
		m_deleteMissileIcon = true;
	}
	
}