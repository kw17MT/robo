#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"
#include "SoundSource.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

namespace
{
	const Vector3 SCALE_CHANGE_AMOUNT = { 0.1f,0.1f,0.1f };
	const float SE_VOLUME = 1.0f;
}

Reticle::~Reticle()
{
	DeleteGO(m_spriteRender[enNormalReticle]);
	DeleteGO(m_spriteRender[enTargetReticle]);
}

bool Reticle::Start()
{
	m_spriteRender[enNormalReticle] = NewGO<SpriteRender>(0);
	m_spriteRender[enNormalReticle]->Init("Assets/Image/reticle/reticle_idle.dds", 20, 20);
	m_spriteRender[enNormalReticle]->SetPosition(m_homePosition);

	m_spriteRender[enTargetReticle] = NewGO<SpriteRender>(0);
	m_spriteRender[enTargetReticle]->Init("Assets/Image/reticle/reticle_target.dds", 96, 96);
	m_spriteRender[enTargetReticle]->SetPosition(m_homePosition);

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

				CSoundSource* selectSE = NewGO<CSoundSource>(0);
				selectSE->Init(L"Assets/sound/lockOn.wav", false);
				selectSE->SetVolume(SE_VOLUME);
				selectSE->Play(false);
			}
			//�N��������^�[�Q�b�g���Ă��鎞
			else if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
			{
				m_isTarget = false;
				CaptureStateManager::GetInstance().SetCaptureState(None);
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
			if (CaptureStateManager::GetInstance().GetMissileTargetState() != enFull) {
				//���������b�N�I�������Ȃ�
				if (CaptureStateManager::GetInstance().GetMissileTargetState() == enMissileTargeted)
				{
					//���P�b�g�^�[�Q�b�g���J�n���鎞�Ԃɖ߂�
					m_PressButtonTime = 0.499f;
					CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);

					CSoundSource* selectSE = NewGO<CSoundSource>(0);
					selectSE->Init(L"Assets/sound/missileLock.wav", false);
					selectSE->SetVolume(SE_VOLUME);
					selectSE->Play(false);
				}
				else
				{
					CaptureStateManager::GetInstance().SetMissileTargetState(enMissileTarget);
				}
			}
		}
		else
		{
			//1�t���[���������P�b�g�̃^�[�Q�b�g�������̂Ń^�[�Q�b�g��Ԃ𖳂��̏�Ԃ�
			CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
		}
	}
}

void Reticle::CalcPosition()
{
	if (m_isTarget)
	{
		//�^�[�Q�b�g����G�̈ʒu���X�N���[�����W�ɕϊ�����B
		CalcMethods::CalcScreenPos(m_lockOnPosition, m_targetingEnemyPos);
		//���̈ʒu���J�����̌�둤�ɗ���Ȃ�
		if (m_lockOnPosition.z == -1.0f)
		{
			//�^�[�Q�b�g������
			m_isTarget = false;
			//�v���C���[�̃^�[�Q�b�g��Ԃ���������B
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}

		//���b�N�I���p�̉摜�ʒu��ݒ�
		m_spriteRender[enTargetReticle]->SetPosition(m_lockOnPosition);

		//���b�N�I�����Ă�����X�P�[����1�ɖ߂��Ă���
		m_reticleScale[enTargetReticle] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enTargetReticle].x >= 1.0f)
		{
			m_reticleScale[enTargetReticle] = Vector3::One;
		}
		//�ʏ�̃��e�B�N���͏��������Ă���
		m_reticleScale[enNormalReticle] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enNormalReticle].x <= 0.0f)
		{
			m_reticleScale[enNormalReticle] = Vector3::Zero;
		}
	}
	//�^�[�Q�b�g���Ă��Ȃ����
	else
	{
		m_reticleScale[enTargetReticle] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enTargetReticle].x <= 0)
		{
			m_reticleScale[enTargetReticle] = Vector3::Zero;
		}

		m_reticleScale[enNormalReticle] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enNormalReticle].x >= 1)
		{
			m_reticleScale[enNormalReticle] = Vector3::One;
		}
	}
}

void Reticle::Update()
{
	//�v���C���[�̃^�[�Q�b�g��Ԃ������Ȃ��Ȃ��
	if (CaptureStateManager::GetInstance().GetCaptureState() != Targeted)
	{
		//���e�B�N���̓^�[�Q�b�g���Ă��Ȃ�
		m_isTarget = false;
	}

	if (CaptureStateManager::GetInstance().GetCaptureState() == ChangeMainTarget
		&& !CaptureStateManager::GetInstance().GetIsDecidedNextTarget())
	{
		CaptureStateManager::GetInstance().SetCaptureState(None);
	}

	//LB2�{�^���������Ă��邩�Ń^�[�Q�b�g��Ԃ�ݒ肷��B
	ChangeTargetStateDependOnButtonLB2();

	//���b�N�I�����e�B�N���̈ʒu�A�g�嗦�ݒ�B�ʏ탌�e�B�N���̊g�嗦�ݒ�
	CalcPosition();

	m_spriteRender[enNormalReticle]->SetScale(m_reticleScale[enNormalReticle]);
	m_spriteRender[enTargetReticle]->SetScale(m_reticleScale[enTargetReticle]);
	m_spriteRender[enNormalReticle]->SetPosition(m_homePosition);

	m_targetReticleRotateRate += 1.0f;
	Quaternion targetReticleRot;
	targetReticleRot.SetRotationDegZ(m_targetReticleRotateRate);
	m_spriteRender[enTargetReticle]->SetRotation(targetReticleRot);
}