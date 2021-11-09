#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"

namespace
{
	const int WAIT_SECOND = 1.0f;
	const int MAX_LAUNCH_NUM = 10;
	const float PI = 3.14f;
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
	return true;
}

void MissileGenerator::Update()
{
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

			//individualPos.x += sin(2 * PI * (-2 + i / 10.0f)) * 100.0f;
			//individualPos.y += cos(2 * PI * ( 0.25f * (i + 1) / 10.0f)) * 300.0f;

			//float a = sin(PI / (i + 1));
			//float b = cos(PI / (i + 1));

			m_missiles.back()->SetTargetAndCurrentPos(CaptureStateManager::GetInstance().GetRocketTargetEnemyPos(i), individualPos);
			m_missiles.back()->SetEnemy(CaptureStateManager::GetInstance().GetRocketTargetEnemy(i));
			m_missiles.back()->SetNumber(i);
		}
		//���P�b�g�𔭎˂������߁A���b�N�I�����Ă����G�̐��Əꏊ�����Z�b�g
		CaptureStateManager::GetInstance().ResetRocketTargetParam();

		m_isPrepareLaunch = false;
		m_isLaunch = false;
	}
}