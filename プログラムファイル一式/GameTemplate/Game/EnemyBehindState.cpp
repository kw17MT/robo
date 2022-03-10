#include "stdafx.h"
#include "EnemyBehindState.h"
#include "EnemyDashState.h"
#include "EnemyApproachState.h"

void EnemyBehindState::Init(EnemyStateParam &param)
{
	//���Ԃ����Z�b�g
	param.timer = 0.0f;
	//�w����Ƃ葱����悤�ɂ���
	param.getBehind = true;
}

EnemyMoveState* EnemyBehindState::Update(EnemyStateParam &param)
{
	//�v���C���[�����Ă�������ƓG����v���C���[�ւ̕����̓��ς��v�Z����B
	float dot = param.toTargetDirection.Dot(g_camera3D->GetForward());
	//���ς����ȉ��Ȃ�Ό����Ă���Ɣ��f����
	if (dot < behind::DOT_RECOGNIZE_PLAYER)
	{
		//�_�b�V����Ԃɂ���B
		return EnemyDashState::GetInstance();
	}
	else
	{
		//�J��Ԃ��w����Ƃ郂�[�h����߂�B
		param.getBehind = false;
		//�ڋߏ�Ԃɂ���B
		return EnemyApproachState::GetInstance();
	}

	return this;
}