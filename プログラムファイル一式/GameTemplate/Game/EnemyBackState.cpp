#include "stdafx.h"
#include "EnemyBackState.h"
#include "EnemyApproachState.h"
#include "EnemyAroundState.h"

namespace
{
	const int USE_STATE_NUM = 2;						//�����Ŏg�p�����Ԃ̐�
}

void EnemyBackState::Init(EnemyStateParam &param)
{
	//�o�ߎ��Ԃ����Z�b�g
	param.timer = 0.0f;
}

EnemyMoveState* EnemyBackState::Update(EnemyStateParam &param)
{
	//�G�ƃv���C���[�����ꂷ�����
	if (param.distanceBetweenEnemyToPlayer >= approach::LIMIT_RECOGNIZE)
	{
		//�����_���Őڋߏ�Ԃɂ��邩�A�����Ԃɂ��邩�I������B
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % USE_STATE_NUM;
		if (nextMoveState)
		{
			//�ڋ�
			return EnemyApproachState::GetInstance();
		}
		else if (!nextMoveState)
		{
			//����
			return EnemyAroundState::GetInstance();
		}
	}

	return this;
}