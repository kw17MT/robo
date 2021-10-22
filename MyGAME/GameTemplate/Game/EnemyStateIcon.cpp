#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"

namespace
{
	const float FAR_DISTANCE = 80000.0f;					//���b�N�I���\�ȋ����B�l�p�̉摜���o���^�C�~���O�̋���
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//�ߑ����e�B�N���̎��k��
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//�ߑ����e�B�N���̏����g�嗦
}

EnemyStateIcon::~EnemyStateIcon()
{
	DeleteGO(m_spriteRender[0]); m_spriteRender[0] = nullptr;
	DeleteGO(m_spriteRender[1]); m_spriteRender[1] = nullptr;
}

bool EnemyStateIcon::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_toofar.dds",48,48);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_near2.dds",48,48);

	return true;
}

void EnemyStateIcon::JudgeState(int distance)
{
	//���̓G�����b�N�I������Ă��鎞�͑��̓G�̏�Ԃ�ς��Ȃ�
	if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
	{
		return;
	}
	//���b�N�I���ł��鋗���Ȃ��
	if (distance < FAR_DISTANCE)                              
	{
		m_enemyState = enemyNear;
	}
	//���b�N�I���ł��Ȃ������Ȃ��
	else if(distance >= FAR_DISTANCE)
	{
		m_enemyState = enemyTooFar;
	}
}

void EnemyStateIcon::CalcPosition()
{
	//�Ή�����G�̃|�W�V��������X�N���[�����W�����߂�
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, m_enemyPos);
	m_position.x = -m_screenPos.x;
	m_position.y = m_screenPos.y;
	//�G�̈ʒu�ƃJ�����̑O�����̓��ςɂ���ăA�C�R�����f�������߂�
	//�G����J�����ւ̃x�N�g���쐬
	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//���K��
	enemyToCamera.Normalize();
	//�G�̈ʒu�ƃJ�����̑O�����̓���
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//�G���J�����̑O�����ɂ���Ȃ�Ήf��
	if (dot < 0.0f)
	{
		m_position.z = 0.0f;
	}
	//��둤�ɂ���
	else
	{
		m_position.z = -1.0f;
	}
}

void EnemyStateIcon::DisplayIcons()
{
	//�X�N���[�����W����ʂ̒����t�߂Ȃ�
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f)
	{
		//�v���C���[�������ߑ����Ă��Ȃ����
		if (CaptureStateManager::GetInstance().GetCaptureState() == None) {
			//�ߑ����e�B�N���i�l�p�j�������B
			m_isCaptured = true;
			m_isFirstExpand = false;
			//�ߑ����ꂽ�G�����邱�ƂƁA���̈ʒu���W��ۑ�
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}
	}
	//�͈͂���͂����
	else
	{
		//���ꂪ�ߑ����̓G�Ȃ��
		if (m_isCaptured)
		{
			//�ߑ���ԉ���
			m_isCaptured = false;
			//�ߑ�����Ă���G�͂��Ȃ���Ԃɂ���B
			CaptureStateManager::GetInstance().SetCaptureState(None);

			Vector3 frontRobo =  g_camera3D->GetTarget() - g_camera3D->GetPosition();
			frontRobo.Normalize();
			CaptureStateManager::GetInstance().SetCapturedEnemyPos(frontRobo * 10.0f);
		}

		//�ߑ����e�B�N���̏����̊g���������x�������̂Ńt���O��߂�
		m_isFirstExpand = false;
		//�ߑ����e�B�N���k��
		m_scale[1] = Vector3::Zero;
	}
}

void EnemyStateIcon::IconBehaviour()
{
	//�o�c�A�C�R���͊�{�o���B���b�N�I�����ꂽ���Ɋg�嗦���X�V����B
	m_scale[0] = Vector3::One;

	switch (m_enemyState)
	{
		//�����̓��b�N�I�������\���ȋ����̒��ŁA�ߑ�����Ă��鎞
	case enemyNear:
		if (m_isCaptured)
		{
			//�n�߂̊g�嗦��傫�����Ă���
			if (m_isFirstExpand == false)
			{
				//�傫�߂̊g�嗦�ɍX�V
				m_scale[1] = APPEAR_RATE;
				//�������߂��Ȃ�����ŏ����������傫�߂ɂ���B
				m_spriteRender[1]->SetScale(m_scale[1]);
				//�ŏ��̊g�傪�I�����
				m_isFirstExpand = true;
				return;
			}
			//�傫�����Ă������̂����񂾂񏬂������Ă����B
			m_scale[1] -= SHRINK_RATE;
			if (m_scale[1].x < 1.0f)
			{
				m_scale[1] = Vector3::One;
			}

			//�v���C���[���G���^�[�Q�b�g���Ă���Ȃ�΂���͎���
			if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
			{
				m_enemyState = enemyTargeted;
			}
		}
		break;

	case enemyTooFar:
		//�G�������Ȃ����̂ŕߑ����e�B�N���̊g�嗦���[���ɂ��ď���
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		if (m_isCaptured) {
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		m_isCaptured = false;
		break;

	case enemyTargeted:
		//���̃A�C�R�����ǐ����Ă���G�����b�N�I�����ꂽ�̂�2��ނ̃A�C�R���������B
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(m_enemyPos);
		break;
	}
}

void EnemyStateIcon::Update()
{
	CalcPosition();
	
	DisplayIcons();

	//�G�ƃv���C���[�̋����ɂ���ĐU�镑����ς���B
	IconBehaviour();

	//�o�c�̃��e�B�N���̊g�嗦�X�V
	m_spriteRender[0]->SetScale(m_scale[0]);
	//�ߑ����e�B�N���̊g�嗦�X�V
	m_spriteRender[1]->SetScale(m_scale[1]);
	//���e�B�N����G�̃X�N���[�����W�ɒu���B
	m_spriteRender[0]->SetPosition(m_position);
	m_spriteRender[1]->SetPosition(m_position);
}