#pragma once

namespace
{
	namespace dash
	{
		const float MAX_TIME = 5.0f;						//�_�b�V���̍ő�p������
	}
	namespace around
	{
		const float MAX_TIME = 6.0f;						//����̍ő�p������
	}
	namespace behind
	{
		const float DOT_RECOGNIZE_PLAYER = -0.85f;			//�v���C���[�����Ă���Ɗ��������
	}
	namespace approach
	{
		const float LIMIT_APPROACH = 2000.0f;				//�v���C���[�ɋ߂Â��鋗��
		const float LIMIT_RECOGNIZE = 7000.0f;				//�v���C���[��F���ł��鋗��
	}	
	namespace stay
	{
		const float MAX_TIME = 10.0f;						//�ҋ@�̍ő�p������
	}
	namespace stateNo
	{
		const int DASH = 0;									//�_�b�V���̔ԍ�
		const int AROUND = 1;								//����̔ԍ�
		const int BACK = 2;									//��ނ̔ԍ�
		const int BEHIND = 3;								//�w����Ƃ��Ԃ̔ԍ�
		const int APPROACH = 4;								//�ڋߏ�Ԃ̔ԍ�
		//const int STAY = 5;
	}
}

struct EnemyStateParam
{
	EnemyStateParam()
	{
		timer = 0.0f;										
		distanceBetweenEnemyToPlayer = 0.0f;				
		toTargetDirection = Vector3::Zero;					
		getBehind = false;									
	}

	float timer;											//�o�ߎ���
	float distanceBetweenEnemyToPlayer;						//�G�ƃv���C���[�̋���
	Vector3 toTargetDirection;								//�^�[�Q�b�g�ւ̌���
	bool getBehind;											//�w����p�����Ď��ɍs����
};