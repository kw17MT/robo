#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"

extern Vector3 CalcScreenPos(Vector3& screenPos, Vector3 pos);

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

void Reticle::ChangeTargetStateDependOnButtonLB2()
{
	//LB2�{�^���������Ă��鎞�Ԃ��v������B
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		m_PressButtonTime -= GameTime().GetFrameDeltaTime();
	}
	//LB2�{�^����������Ă��Ȃ����
	if (!g_pad[0]->IsPress(enButtonLB2))
	{
		//���傢�������炢�Ȃ�Βʏ탍�b�N�I����Ԃ�ω�������B
		if (m_PressButtonTime <= 0.99f && m_PressButtonTime >= 0.5f)
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
		//LB2�{�^���������ꂽ�牟�����Ԃ����Z�b�g����B
		m_PressButtonTime = 1.0f;
	}

	//����LB2�{�^���������Ă���Ȃ�A���P�b�g�̃^�[�Q�e�B���O�J�n
	if (m_PressButtonTime < 0.5f)
	{
		//�������Ԃ����ȏ�ɂȂ�΃��P�b�g�̃^�[�Q�b�g�J�n
		if (m_PressButtonTime <= 0.0f) {
			//�^�[�Q�b�g�̍ő吔�łȂ����
			if (CaptureStateManager::GetInstance().GetRocketTargetNum() != 9) {
				CaptureStateManager::GetInstance().SetRocketTargetState(true);

				//���������b�N�I�������Ȃ�
				if(CaptureStateManager::GetInstance().GetIsRocketTargeted()){
				//���P�b�g�^�[�Q�b�g���J�n���鎞�Ԃɖ߂�
				m_PressButtonTime = 0.499f;
				CaptureStateManager::GetInstance().SetIsRocketTargeted(false);
				}
			}
		}
		else
		{
			//1�t���[���������P�b�g�̃^�[�Q�b�g�������̂Ń^�[�Q�b�g��Ԃ𖳂��̏�Ԃ�
			CaptureStateManager::GetInstance().SetRocketTargetState(false);
		}
	}
}

void Reticle::CalcPosition()
{
	if (m_isTarget)
	{
		//�^�[�Q�b�g����G�̈ʒu���X�N���[�����W�ɕϊ�����B
		Vector3 targetPos = CalcScreenPos(m_lockOnPosition, CaptureStateManager::GetInstance().GetCapturedEnemyPos());
		//���̈ʒu���J�����̌�둤�ɗ���Ȃ�
		if (targetPos.z == -1.0f)
		{
			//�^�[�Q�b�g������
			m_isTarget = false;
			//�v���C���[�̃^�[�Q�b�g��Ԃ���������B
			CaptureStateManager::GetInstance().SetCaptureState(None);
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
	//�v���C���[�̃^�[�Q�b�g��Ԃ������Ȃ��Ȃ��
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		//���e�B�N���̓^�[�Q�b�g���Ă��Ȃ�
		m_isTarget = false;
		//����܂Ń^�[�Q�b�g���Ă����G�̈ʒu�����Z�b�g����B
		Vector3 homePos = { 0.0f,0.0f,-1.0f };
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(homePos);
	}

	//LB2�{�^���������Ă��邩�Ń^�[�Q�b�g��Ԃ�ݒ肷��B
	ChangeTargetStateDependOnButtonLB2();

	//���b�N�I�����e�B�N���̈ʒu�A�g�嗦�ݒ�B�ʏ탌�e�B�N���̊g�嗦�ݒ�
	CalcPosition();

	m_spriteRender[0]->SetScale(m_reticleScale[0]);
	m_spriteRender[1]->SetScale(m_reticleScale[1]);
	m_spriteRender[0]->SetPosition(m_homePosition);
}