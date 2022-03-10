#include "stdafx.h"
#include "EnemyAroundState.h"
#include "EnemyApproachState.h"

void EnemyAroundState::Init(EnemyStateParam &param)
{
	//�o�ߎ��Ԃ����Z�b�g����
	param.timer = 0.0f;
}

EnemyMoveState* EnemyAroundState::Update(EnemyStateParam &param)
{
	//�f���^�^�C���Ōv��
	param.timer += GameTime().GetFrameDeltaTime();
	//��莞�Ԑ��񂵂���
	if (param.timer >= around::MAX_TIME)
	{
		//�ڋߏ�ԂɑJ�ڂ���
		return EnemyApproachState::GetInstance();
	}

	return this;
}