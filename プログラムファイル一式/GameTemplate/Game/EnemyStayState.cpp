#include "stdafx.h"
#include "EnemyStayState.h"
#include "EnemyDashState.h"

void EnemyStayState::Init(EnemyStateParam &param)
{
	//�o�ߎ��Ԃ����Z�b�g����B
	param.timer = 0.0f;
}

EnemyMoveState* EnemyStayState::Update(EnemyStateParam &param)
{
	//�f���^�^�C���Ōo�ߎ��Ԃ��v��
	param.timer += GameTime().GetFrameDeltaTime();
	//�ҋ@���Ԃ𒴂�����
	if (param.timer >= stay::MAX_TIME)
	{
		//�_�b�V�����s���B
		return EnemyDashState::GetInstance();
	}

	return this;
}