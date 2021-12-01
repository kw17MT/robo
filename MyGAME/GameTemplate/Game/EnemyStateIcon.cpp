#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"
#include "MissileTargetIcon.h"
#include "CrossIcon.h"
#include "CapturedSquareIcon.h"
#include "Reticle.h"

//FindGO�Ώ�
#include "MissileGenerator.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

namespace
{
	const float FAR_DISTANCE = 80000.0f;					//���b�N�I���\�ȋ����B�l�p�̉摜���o���^�C�~���O�̋���
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//�ߑ����e�B�N���̎��k��
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//�ߑ����e�B�N���̏����g�嗦
	const float CAPTURE_HEIGHT = 80.0f;						//�ߑ��ł����ʏ�̍���
	const float CAPTURE_WIDTH = 45.0f;						//�ߑ��ł����ʏ�̉���
	const float NEXT_TARGET_AREA_HEIGHT = 200.0f;			//�������^�[�Q�b�g���锻����s���G���A�̍���
	const float NEXT_TARGET_AREA_WIDTH = 150.0f;			//�������^�[�Q�b�g���锻����s���G���A�̉���
}

EnemyStateIcon::~EnemyStateIcon()
{
	//�o�c�摜���폜
	DeleteGO(m_crossIcon); m_crossIcon = nullptr;
	//�ߑ��̉摜���폜
	DeleteGO(m_squareIcon); m_squareIcon = nullptr;

	//�~�T�C���̃^�[�Q�b�g�A�C�R����S�폜
	for (auto i : m_missileTargetIcon)
	{
		DeleteGO(m_missileTargetIcon.back());
		m_missileTargetIcon.pop_back();
	}
	m_missileTargetIcon.clear();

	if (m_nextTarget)
	{
		//m_reticle->SetIsDecidedNextTarget(false);
		CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
	}
	if (m_isCaptured)
	{
		//m_reticle->SetIsDecidedNextTarget(false);
		CaptureStateManager::GetInstance().SetCaptureState(None);
	}
	if (m_enemyState == enemyTargeted)
	{
		CaptureStateManager::GetInstance().SetCaptureState(ChangeMainTarget);
	}
}

bool EnemyStateIcon::Start()
{
	//�o�c�摜�𐶐�
	m_crossIcon = NewGO<CrossIcon>(0);
	//�ߑ��摜�𐶐�
	m_squareIcon = NewGO<CapturedSquareIcon>(0);
	//�~�T�C����������쐬
	m_missileGenerator = FindGO<MissileGenerator>("missileGene");

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
	if (m_screenPos.x > -CAPTURE_HEIGHT && m_screenPos.x < CAPTURE_HEIGHT 
		&& m_screenPos.y > -CAPTURE_WIDTH && m_screenPos.y < CAPTURE_WIDTH)
	{
		//�v���C���[�������ߑ����Ă��Ȃ����
		if (CaptureStateManager::GetInstance().GetCaptureState() == None
			&& m_enemyState == enemyNear) {
			//�ߑ����e�B�N���i�l�p�j�������B
			m_isCaptured = true;
			m_isFinishedFirstExpand = false;
			//�ߑ����ꂽ�G�����邱�Ƃ�ۑ�
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}

		//���P�b�g�̃^�[�Q�b�g�����閽�߂����Ă�����
		if (CaptureStateManager::GetInstance().GetMissileTargetState() == enMissileTarget) {
			//�~�T�C���������Əo�����ԂȂ��
			if (m_missileGenerator->CanTargetMore())
			{
				//�^�[�Q�b�g����G�̃C���X�^���X��ۑ�
				m_missileGenerator->SaveTargetedEnemy(m_enemy);
				
				//���P�b�g�Ƀ^�[�Q�b�g���ꂽ�G�̐���z��̗v�f���ɗ��p����
				m_missileTargetIcon.push_back(NewGO<MissileTargetIcon>(0));
				//�ŏ��̊g����s��
				m_missileTargetIcon.back()->SetFirstExpandScale(true);
				//�A�C�R�����o���Ώۂ̓G�̃C���X�^���X��ۑ�
				m_missileTargetIcon.back()->SetTargetedEnemy(m_enemy);
				//�~�T�C���̃^�[�Q�b�g���s����
				CaptureStateManager::GetInstance().SetMissileTargetState(enMissileTargeted);
			}
			//�~�T�C���͂����o���Ȃ��ꍇ
			else
			{
				//�ő���~�T�C�����b�N�I��������Ԃɂ���B
				CaptureStateManager::GetInstance().SetMissileTargetState(enFull);
			}
		}
	}
	//�͈͂���͂����
	else if(m_screenPos.x > -NEXT_TARGET_AREA_HEIGHT && m_screenPos.x < NEXT_TARGET_AREA_HEIGHT 
		&& m_screenPos.y > -NEXT_TARGET_AREA_WIDTH && m_screenPos.y < NEXT_TARGET_AREA_WIDTH)
	{
		//���ꂪ�ߑ����̓G�Ȃ��
		if (m_isCaptured)
		{
			//�ߑ���ԉ���
			m_isCaptured = false;
			//�ߑ�����Ă���G�͂��Ȃ���Ԃɂ���B
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		//�v���C���[�͒N����������b�N�I�����Ă���Ȃ�A���̃��b�N�I�����\�񂵂Ă���
		if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted
			&& !CaptureStateManager::GetInstance().GetIsDecidedNextTarget()/*m_reticle->GetIsDecidedNextTarget() == false*/) {
			//�������g���Ăю��̃��b�N�I���Ώۂɂ��Ȃ��悤�ɂ���B
			if (m_enemyState != enemyTargeted) {
				//���̗\��͂��̓G�ɂ���
				m_nextTarget = true;
				//m_reticle->SetIsDecidedNextTarget(true);
				CaptureStateManager::GetInstance().SetIsDecidedNextTarget(true);
			}
		}

		//�ߑ����e�B�N���̏����̊g���������x�������̂Ńt���O��߂�
		m_isFinishedFirstExpand = false;
		//�ߑ����e�B�N���k��
		m_scale[1] = Vector3::Zero;
	}
	//��ʒ[�t�߂̃G���A
	else
	{
		if (m_nextTarget)
		{
			m_nextTarget = false;
			
			//m_reticle->SetIsDecidedNextTarget(false);
			CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
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
			if (m_isFinishedFirstExpand == false)
			{
				//�傫�߂̊g�嗦�ɍX�V
				m_scale[1] = APPEAR_RATE;
				//�������߂��Ȃ�����ŏ����������傫�߂ɂ���B
				m_squareIcon->SetScale(m_scale[1]);
				//�ŏ��̊g�傪�I�����
				m_isFinishedFirstExpand = true;
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
		m_isFinishedFirstExpand = false;
		if (m_isCaptured) {
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		m_isCaptured = false;
		break;

	case enemyTargeted:
		//���̃A�C�R�����ǐ����Ă���G�����b�N�I�����ꂽ�̂�2��ނ̃A�C�R���������B
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFinishedFirstExpand = false;
		m_isCaptured = false;

		m_reticle->SetIsTargeting(true);

		break;
	}
}

void EnemyStateIcon::Update()
{
	//�ŏ��ɓG�̃X�N���[�����W���X�V����B
	CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	
	//�A�C�R����\������
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
			//�^�[�Q�b�g����Ă���
			m_enemyState = enemyTargeted;
			//���e�B�N���̓^�[�Q�b�g��
			m_reticle->SetIsTargeting(true);
			//m_reticle->SetIsDecidedNextTarget(false);
			CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
			//�v���C���[�̓^�[�Q�b�g���Ă���
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
	}
	//�~�T�C���̃A�C�R������������ׂ��Ȃ��
	if (m_missileGenerator->GetDeleteMissileIcon())
	{
		//�A�C�R����S����
		for (auto i : m_missileTargetIcon)
		{
			DeleteGO(m_missileTargetIcon.back());
			m_missileTargetIcon.pop_back();
		}
		m_missileTargetIcon.clear();
	}
	//�v���C���[�͉����^�[�Q�b�g��ߑ����Ă��Ȃ����
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		//���Ƀ^�[�Q�b�g����Ώۂɂ͂��Ȃ�
		m_nextTarget = false;
		CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
	}

	if (m_enemyState == enemyTargeted)
	{
		//���̓G���^�[�Q�b�g����Ă���Ȃ�΁A���݈ʒu�𑗂�
		m_reticle->SetTargetedEnemyPos(m_enemy->GetPosition());
	}

	//�o�c�̃��e�B�N���̊g�嗦�X�V
	m_crossIcon->SetScale(m_scale[0]);
	//�ߑ����e�B�N���̊g�嗦�X�V
	m_squareIcon->SetScale(m_scale[1]);
	//���e�B�N����G�̃X�N���[�����W�ɒu���B
	m_crossIcon->SetPosition(m_screenPos);
	m_squareIcon->SetPosition(m_screenPos);
}