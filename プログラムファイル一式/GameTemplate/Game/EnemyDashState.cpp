#include "stdafx.h"
#include "EnemyDashState.h"
#include "EnemyApproachState.h"
#include "EnemyBehindState.h"

void EnemyDashState::Init(EnemyStateParam &param)
{
	//�o�ߎ��Ԃ����Z�b�g
	param.timer = 0.0f;
}

EnemyMoveState* EnemyDashState::Update(EnemyStateParam &param)
{
	//�f���^�^�C���Ŏ��Ԃ��v��
	param.timer += GameTime().GetFrameDeltaTime();
	//�_�b�V�����Ԃ��ő厞�Ԃ𒴂�����
	if (param.timer >= dash::MAX_TIME)
	{
		//�v���C���[�͓G�����Ă��邩���ς��g���Ĕ��f����
		float dot = param.toTargetDirection.Dot(g_camera3D->GetForward());
		//���Ă��āA�J��Ԃ��w�����郂�[�h�ł�������
		if (dot < behind::DOT_RECOGNIZE_PLAYER
			&& param.getBehind)
		{
			//�w�������Ԃɂ���
			return EnemyBehindState::GetInstance();
		}
		//�ڋߏ�Ԃɂ���
		return EnemyApproachState::GetInstance();
	}

	return this;
}