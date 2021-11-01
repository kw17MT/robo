#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"

namespace
{
	const int WAIT_SECOND = 1.0f;
	const int MAX_LAUNCH_NUM = 10;
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
		////���b�N�I���̊Ԋu���v������
		//m_lockOnInterval += GameTime().GetFrameDeltaTime();

		////�v�����Ԃ����ȏ�ɂȂ�����
		//if (m_lockOnInterval >= WAIT_SECOND)
		//{
		//	//���˂��郍�P�b�g�̐��𑝂₷
		//	m_launchNum++;

		//	//�����Ń��b�N�I�������ʒu��ݒ肵�Ă�������

		//	//���ˊԊu���Ԃ����Z�b�g
		//	m_lockOnInterval = 0.0f;
		//	//���˂���~�T�C���̍ő吔�ɂȂ��
		//	if (m_launchNum >= MAX_LAUNCH_NUM)
		//	{
		//		//�ő吔���ێ�
		//		m_launchNum = MAX_LAUNCH_NUM;
		//	}
		//}



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
			individualPos.x += i * 100.0f;
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