#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"

namespace
{
	const int FAR_DISTANCE = 80000;							//���b�N�I���\�ȋ����B�l�p�̉摜���o���^�C�~���O�̋���
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//�ߑ����e�B�N���̎��k��
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//�ߑ����e�B�N���̏����g�嗦
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
	if (distance < FAR_DISTANCE)                              
	{
		m_enemyState = enemyNear;
	}
	else if(distance >= FAR_DISTANCE)
	{
		m_enemyState = enemyTooFar;
	}
}

void EnemyStateIcon::IconBehaviour()
{
	switch (m_enemyState)
	{
	case enemyNear:
		if (m_isCaptured && CaptureStateManager::GetInstance().GetOtherCapturedState())
		{
			//�n�߂̊g�嗦��傫�����Ă���
			if (m_isFirstExpand == false)
			{
				//�������߂��Ȃ�����ŏ����������傫�߂ɂ���B
				m_spriteRender[1]->SetScale(APPEAR_RATE);
				//�傫�߂̊g�嗦�ɍX�V
				m_scale[1] = APPEAR_RATE;
				//�ŏ��̊g�傪�I�����
				m_isFirstExpand = true;
				return;
			}
			//�傫�����Ă������̂����񂾂񏬂������Ă����B
			m_scale[1] -= SHRINK_RATE;
			if (m_scale[1].x < 1)
			{
				m_scale[1] = Vector3::One;
			}
		}
		break;
	case enemyTooFar:
		//�G�������Ȃ����̂ŕߑ����e�B�N���̊g�嗦���[���ɂ��ď���
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetOtherCapturedState(false, Vector3::Zero);
		break;
	case enemyTargeted:
		//���̃A�C�R�����ǐ����Ă���G�����b�N�I�����ꂽ�̂�2��ނ̃A�C�R���������B
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetOtherCapturedState(true, m_enemyPos);
		break;
	}
}

void EnemyStateIcon::Update()
{
	//�Ή�����G�̃|�W�V��������X�N���[�����W�����߂�
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, m_enemyPos);
	m_position.x = -m_screenPos.x;
	m_position.y = m_screenPos.y;
	//�X�N���[�����W����ʂ̒����t�߂Ȃ�
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f )
	{
		if (!CaptureStateManager::GetInstance().GetOtherCapturedState()) {
			//�v���C���[�������ߑ����Ă��Ȃ����
			//�ߑ����e�B�N���i�l�p�j�������B
			m_isCaptured = true;
			CaptureStateManager::GetInstance().SetOtherCapturedState(true, m_enemyPos);
		}
	}
	else 
	{
		//�����̊g���������x�������̂Ńt���O��߂�
		m_isFirstExpand = false;
		//�k�����ď���
		m_scale[1] = Vector3::Zero;
		if (m_isCaptured)
		{
			CaptureStateManager::GetInstance().SetOtherCapturedState(false, Vector3::Zero);
		}
		//�ߑ����e�B�N���������Ȃ��B
		m_isCaptured = false;
	}

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