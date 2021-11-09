#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"
#include "RocketTargetStateIcon.h"

extern Vector3 CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

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

	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_rocketTargetIcon[i]);
	}
}

bool EnemyStateIcon::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_toofar.dds",48,48);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_near2.dds",48,48);

	for (int i = 0; i < 10; i++)
	{
		m_rocketTargetIcon[i] = NewGO<RocketTargetStateIcon>(0);
	}

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

void EnemyStateIcon::DisplayIcons()
{
	//�X�N���[�����W����ʂ̒����t�߂Ȃ�
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f)
	{
		//�v���C���[�������ߑ����Ă��Ȃ����
		if (CaptureStateManager::GetInstance().GetCaptureState() == None
			&& m_enemyState == enemyNear) {
			//�ߑ����e�B�N���i�l�p�j�������B
			m_isCaptured = true;
			m_isFirstExpand = false;
			//�ߑ����ꂽ�G�����邱�ƂƁA���̈ʒu���W��ۑ�
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}

		//���P�b�g�̃^�[�Q�b�g�����閽�߂����Ă�����
		if (CaptureStateManager::GetInstance().GetRocketTargetState()) {
			//���݁A���̓G�̈ʒu���W�̓}�l�[�W���[�����z��̉��Ԗڂɕۑ����Ă��邩�c�����Ă���
			m_rocketTargetPosNumber.push_back(CaptureStateManager::GetInstance().GetRocketTargetNum());
			//���P�b�g�Ƀ^�[�Q�b�g���ꂽ�G�̐���z��̗v�f���ɗ��p����
			m_rocketTargetIcon[m_rocketTargetPosNumber.back()]->SetFirstExpandScale(true);
			CaptureStateManager::GetInstance().SetRocketTargetedEnemy(m_enemy);
			CaptureStateManager::GetInstance().PlusRockeTargetNum();
			CaptureStateManager::GetInstance().SetRocketTargetState(false);
		}
	}
	//�͈͂���͂����
	else if(m_screenPos.x > -200.0f && m_screenPos.x < 200.0f && m_screenPos.y > -150.0f && m_screenPos.y < 150.0f)
	{
		//���ꂪ�ߑ����̓G�Ȃ��
		if (m_isCaptured)
		{
			//�ߑ���ԉ���
			m_isCaptured = false;
			//�ߑ�����Ă���G�͂��Ȃ���Ԃɂ���B
			CaptureStateManager::GetInstance().SetCaptureState(None);

			//���b�N�I�����Ă����G�̍��W������������
			//Vector3 frontRobo =  g_camera3D->GetTarget() - g_camera3D->GetPosition();
			//frontRobo.Normalize();
			//CaptureStateManager::GetInstance().SetCapturedEnemyPos(frontRobo * 10.0f);
		}
		//�v���C���[�͒N����������b�N�I�����Ă���Ȃ�A���̃��b�N�I�����\�񂵂Ă���
		if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted
			&& CaptureStateManager::GetInstance().IsNextEnemyCaptured() == false) {
			//�������g���Ăю��̃��b�N�I���Ώۂɂ��Ȃ��悤�ɂ���B
			if (m_enemyState != enemyTargeted) {
				//���̗\��͂��̓G�ɂ���
				m_nextTarget = true;
				//�\�񊮗�
				CaptureStateManager::GetInstance().SetNextEnemy(true);
				//�\���̈ʒu���W��ۑ�
				CaptureStateManager::GetInstance().SetNextEnemyPos(m_enemyPos);
			}
		}

		//�ߑ����e�B�N���̏����̊g���������x�������̂Ńt���O��߂�
		m_isFirstExpand = false;
		//�ߑ����e�B�N���k��
		m_scale[1] = Vector3::Zero;
	}
	//��ʒ[�t�߂̃G���A
	else
	{
		if (m_nextTarget)
		{
			m_nextTarget = false;
			CaptureStateManager::GetInstance().SetNextEnemy(false);
			//�\���̈ʒu���W��ۑ�
			Vector3 homePos = { 0.0f,0.0f,-1.0f };
			CaptureStateManager::GetInstance().SetNextEnemyPos(homePos);
		}
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
	//�ŏ��ɓG�̃X�N���[�����W���X�V����B
	m_position = CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	
	DisplayIcons();

	//�G�ƃv���C���[�̋����ɂ���ĐU�镑����ς���B
	IconBehaviour();

	//�\�񂵂Ă����G�ɏƏ��𓖂Ă�悤�ɖ��߂����Ă�����
	if (CaptureStateManager::GetInstance().GetCaptureState() == ChangeMainTarget)
	{
		//���g�͗\�񂳂�Ă�����
		if (m_nextTarget)
		{
			//�\��������A
			m_nextTarget = false;
			//�ߑ�����Ă���
			m_isCaptured = false;
			m_enemyState = enemyTargeted;
			//�v���C���[�̓^�[�Q�b�g���Ă���
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
	}

	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		m_nextTarget = false;
	}

	//�}�l�[�W���[���ɕۑ����Ă���ł��낤�Ή������ʒu���W���X�V����B
	for (int i = 0; i < m_rocketTargetPosNumber.size(); i++)
	{
		//��������v�f���Ɏw�肵�āA���������݂̓G�̈ʒu�ōX�V����B
		CaptureStateManager::GetInstance().SetRocketTargetPos(m_enemyPos, m_rocketTargetPosNumber[i]);
	}

	//�o�c�̃��e�B�N���̊g�嗦�X�V
	m_spriteRender[0]->SetScale(m_scale[0]);
	//�ߑ����e�B�N���̊g�嗦�X�V
	m_spriteRender[1]->SetScale(m_scale[1]);
	//���e�B�N����G�̃X�N���[�����W�ɒu���B
	m_spriteRender[0]->SetPosition(m_position);
	m_spriteRender[1]->SetPosition(m_position);
}