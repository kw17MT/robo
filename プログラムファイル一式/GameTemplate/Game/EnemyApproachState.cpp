#include "stdafx.h"
#include "EnemyApproachState.h"
#include "EnemyDashState.h"
#include "EnemyAroundState.h"
#include "EnemyStayState.h"
#include "EnemyBehindState.h"
#include "EnemyBackState.h"

namespace
{
	const int USE_STATE_NUM = 4;		//�����Ŏg�p����ʂ̃X�e�[�g�̐�
}

void EnemyApproachState::Init(EnemyStateParam &param)
{
	//�o�ߎ��Ԃ����Z�b�g
	param.timer = 0.0f;
}

EnemyMoveState* EnemyApproachState::Update(EnemyStateParam &param)
{
	//�v���C���[�Ɍ��E�܂ŋ߂Â�����
	if (param.distanceBetweenEnemyToPlayer < approach::LIMIT_APPROACH)
	{
		//�����_���Ɉړ��X�e�[�g��I������
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % USE_STATE_NUM;

		if (nextMoveState == stateNo::DASH)
		{
			//�_�b�V�����
			return EnemyDashState::GetInstance();
		}
		else if (nextMoveState == stateNo::AROUND)
		{
			//������
			return EnemyAroundState::GetInstance();
		}
		else if (nextMoveState == stateNo::BACK)
		{
			//��ޏ��
			return EnemyBackState::GetInstance();
		}
		else if (nextMoveState == stateNo::BEHIND)
		{
			//�v���C���[�̔w��������
			return EnemyBehindState::GetInstance();
		}
	}

	return this;
}