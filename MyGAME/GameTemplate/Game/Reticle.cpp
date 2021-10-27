#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"

namespace
{
	const Vector3 SCALE_CHANGE_AMOUNT = { 0.1f,0.1f,0.1f };
}

Reticle::~Reticle()
{
	DeleteGO(m_spriteRender[0]);
	DeleteGO(m_spriteRender[1]);
}

bool Reticle::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_idle.dds", 48, 48);
	m_spriteRender[0]->SetPosition(m_homePosition);

	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_target.dds", 96, 96);
	m_spriteRender[1]->SetPosition(m_homePosition);

	return true;
}

void Reticle::CalcPosition()
{
	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		//�N�������ߑ����Ă��Ȃ��ƃ��b�N�I���ł��Ȃ��悤��
		if (CaptureStateManager::GetInstance().GetCaptureState() == Captured)
		{
			m_isTarget = true;
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
		//�N��������^�[�Q�b�g���Ă��鎞
		else if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
		{
			m_isTarget = false;
			CaptureStateManager::GetInstance().SetCaptureState(None);
			CaptureStateManager::GetInstance().ResetNextEnemyParam();
		}
	}

	if (m_isTarget)
	{
		//���e�B�N���̍��W���^�[�Q�b�g���ꂽ�G�̈ʒu�ɂ���Č��肷��B
		g_camera3D->CalcScreenPositionFromWorldPosition(
			m_lockOnPosition,
			CaptureStateManager::GetInstance().GetCapturedEnemyPos());

		Vector3 targetPos = Vector3::Zero;
		targetPos.x = -m_lockOnPosition.x;
		targetPos.y = m_lockOnPosition.y;

		Vector3 toCamera = g_camera3D->GetPosition() - m_lockOnPosition;
		//���K��
		toCamera.Normalize();
		//�G�̈ʒu�ƃJ�����̑O�����̓���
		float dot = g_camera3D->GetForward().Dot(toCamera);
		//�G���J�����̑O�����ɂ���Ȃ�Ήf��
		if (dot < 0.0f)
		{
			targetPos.z = 0.0f;
		}
		//��둤�ɂ���
		else
		{
			targetPos.z = -1.0f;
			if (m_isTarget)
			{
				//���b�N�I���̑Ώۂ���ʊO�ɏo�����߁A���b�N�I����؂�
				m_isTarget = false;
				CaptureStateManager::GetInstance().SetCaptureState(None);
			}
		}

		//���b�N�I���p�̉摜�ʒu��ݒ�
		m_spriteRender[1]->SetPosition(targetPos);

		//���b�N�I�����Ă�����X�P�[����1�ɖ߂��Ă���
		m_reticleScale[1] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x >= 1.0f)
		{
			m_reticleScale[1] = Vector3::One;
		}
		//�ʏ�̃��e�B�N���͏��������Ă���
		m_reticleScale[0] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x <= 0.0f)
		{
			m_reticleScale[0] = Vector3::Zero;
		}
	}
	//�^�[�Q�b�g���Ă��Ȃ����
	else
	{
		m_reticleScale[1] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x <= 0)
		{
			m_reticleScale[1] = Vector3::Zero;
		}

		m_reticleScale[0] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x >= 1)
		{
			m_reticleScale[0] = Vector3::One;
		}
	}
}

void Reticle::Update()
{
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		m_isTarget = false;
		Vector3 homePos = { 0.0f,0.0f,-1.0f };
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(homePos);
	}

	//���b�N�I�����e�B�N���̈ʒu�A�g�嗦�ݒ�B�ʏ탌�e�B�N���̊g�嗦�ݒ�
	CalcPosition();

	m_spriteRender[0]->SetScale(m_reticleScale[0]);
	m_spriteRender[1]->SetScale(m_reticleScale[1]);
	m_spriteRender[0]->SetPosition(m_homePosition);
}