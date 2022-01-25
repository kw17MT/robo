#include "stdafx.h"
#include "EnemyMove.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float DOT_ENEMY_RECOGNIZE_PLAYER_FRONT = -0.85f;				//�G���v���C���[�̂ǂ̂��炢���ʂɂ��邩�i���ό��ʁj
	const float MAX_WAIT_TIME = 10.0f;									//Stay���[�h�ł̑ҋ@����ő厞��
	const float MAX_AROUND_TIME = 6.0f;									//���񂷂�ő厞��
	const float MAX_DASH_TIME = 5.0f;									//�_�b�V������ő厞��
	const float ADJUST_ACCERALATION_RATE = 5.0f;						//�����x�̑��������p
	const float LIMIT_ENEMY_CAN_APPROACH = 2000.0f;						//�G���߂Â����Ƃ�����x
	const float LIMIT_ENEMY_RECOGNIZE_PLAYER = 7000.0f;					//�G���v���C���[��F���ł�����E����
	const int MAX_ROTATION_DEGREE = 60;
	const int MOVE_STATE_NUM = 5;
	const float SPEED_DECAY_RATE = 10.0f;
}

void EnemyMove::JudgeMoveType(float distanceBetweenEnemyToPlayer)
{
	//�v���C���[�ɋ߂Â���������
	if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
	{
		//�ړ��^�C�v�������_���ɑI��
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % MOVE_STATE_NUM;

		//�G�������đO��ɓ���
		if (nextMoveState == enFrontAndBehind) {
			m_isCalcEnemyDashDirection = false;
			m_moveType = enFrontAndBehind;
		}
		//�v���C���[�̌����Ƃ�����O��ɓ���
		else if (nextMoveState == enPlayerBehind)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enPlayerBehind;
		}
		//�v���C���[���S�ɐ��񂷂�
		else if (nextMoveState == enAround)
		{
			m_moveType = enAround;
		}
		//�߂Â��ă_�b�V���Œʂ蔲����
		else if (nextMoveState == enApproachAndDash)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enApproachAndDash;
		}
		//�v���C���[�̋߂���Y��
		else // nextMoveState == 4
		{
			m_moveType = enStay;
		}
	}
}

void EnemyMove::CalcApproachSpeed(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	//�O�t���[���̈ړ����������������v���C���[�Ɍ������ړ�����
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);
	//�߂Â������ʁA�ړ��^�C�v��ς���ׂ����ǂ����̔��f
	JudgeMoveType(distanceBetweenEnemyToPlayer);
}

void EnemyMove::CalcFrontAndBehindDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	//�����̓v���C���[�̌��ɂ���̂��O�ɂ���̂�
	float dot = toTargetVec.Dot(g_camera3D->GetForward());
	//�v���C���[�̔w����Ƃ郂�[�h�Ȃ̂ɁA�v���C���[�̐��ʕt�߂ɗ��Ă�����
	if (dot < DOT_ENEMY_RECOGNIZE_PLAYER_FRONT && m_moveType == enPlayerBehind)
	{
		//�_�b�V���ŕ����]������
		m_moveType = enDash;
	}
	else
	{
		//�K��ȏ�Ȃ�
		if (distanceBetweenEnemyToPlayer > LIMIT_ENEMY_RECOGNIZE_PLAYER)
		{
			//�O�t���[���܂ł͌�ރ��[�h�Ȃ�
			if (!m_moveForward)
			{
				//�����x�����Z�b�g����B
				m_acceralation = 0.0f;
			}
			//�O�i���[�h�ɂ���
			m_moveForward = true;

			//�����̊m���Ńv���C���[���S�ɉ�郂�[�h�ɂ���
			bool isRound = rand() % 2;
			if (isRound)
			{
				m_moveType = enAround;
			}
		}
		//�K��̋��������ɂȂ��
		else if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
		{
			//�O�t���[���܂ł͑O�i���[�h�������ꍇ
			if (m_moveForward)
			{
				//�����x�����Z�b�g
				m_acceralation = 0.0f;
			}
			//��ރ��[�h�ɂ���
			m_moveForward = false;
		}

		//�O�i���[�h�������ꍇ�̑��x�v�Z
		if (m_moveForward)
		{
			m_currentMoveDirection = (toTargetVec * m_acceralation) + ((toTargetVec * -1.0f) * (1.0f - m_acceralation));
		}
		//��ރ��[�h�������ꍇ�̑��x�v�Z
		else
		{
			m_currentMoveDirection = (toTargetVec * -1.0f) * m_acceralation + (toTargetVec * (1.0f - m_acceralation));
		}
	}
}

void EnemyMove::CalcAroundDirection(Vector3 toTargetVec)
{
	m_aroundTimer += GameTime().GetFrameDeltaTime();
	if (m_aroundTimer >= MAX_AROUND_TIME)
	{
		m_aroundTimer = 0.0f;
		m_moveType = enApproach;
	}

	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);

	toTargetVec += sideVec;
	toTargetVec *= -1.0f;
	toTargetVec.Normalize();

	//���̃t���[���̈ړ��������L�^
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);;
}

void EnemyMove::CalcApproachAndDashDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
	{
		//�_�b�V���Ńv���C���[�������悤�ɂǂ�����
		m_moveType = enDash;
	}
	else
	{
		m_currentMoveDirection = toTargetVec;
	}

	if (distanceBetweenEnemyToPlayer >= LIMIT_ENEMY_RECOGNIZE_PLAYER)
	{
		m_moveType = enApproach;
	}
}

void EnemyMove::EnemyDashDirection(Vector3 toTargetVec)
{
	//�ŏ��̃_�b�V�������̌v�Z���I����Ă��Ȃ����
	if (!m_isCalcEnemyDashDirection)
	{
		//�������x�N�g���̌v�Z
		Vector3 axisSide = toTargetVec.CalcCross(g_vec3AxisY);
		//������x�N�g���̌v�Z
		Vector3 axisUp = toTargetVec.CalcCross(axisSide);

		//1~90�̐��l�𓾂�
		float degreeZ = rand() % MAX_ROTATION_DEGREE + 1;
		float degreeY = rand() % MAX_ROTATION_DEGREE + 1;
		//���l��-29~30�ɒ��߂���
		degreeZ -= MAX_ROTATION_DEGREE / 2.0f;
		degreeY -= MAX_ROTATION_DEGREE / 2.0f;

		Quaternion qRot;
		//Y�����ւ̉�]���v�Z�A�ړ��x�N�g���ɓK�p����B
		qRot.SetRotationDeg(axisSide, degreeY);
		qRot.Apply(toTargetVec);
		//�����ւ̉�]���v�Z�A�ړ��x�N�g���ɓK�p����B
		qRot.SetRotationDeg(axisUp, degreeZ);
		qRot.Apply(toTargetVec);

		m_dashDirection = toTargetVec;
		m_isCalcEnemyDashDirection = true;
	}
	//�O�̈ړ����x�����������_�b�V���������v�Z
	m_currentMoveDirection = m_dashDirection * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);

	//�_�b�V�����Ă��鎞�Ԃ��v������
	m_dashTimer += GameTime().GetFrameDeltaTime();
	if (m_dashTimer >= MAX_DASH_TIME)
	{
		m_moveType = enApproach;
		m_dashTimer = 0.0f;
		m_waitTimer = 0.0f;
	}
}

void EnemyMove::EnemyStayDirection(Vector3 toTargetVec)
{
	//�Y���Ă��鎞�Ԃ��v��
	m_waitTimer += GameTime().GetFrameDeltaTime();
	//�K��ȏ�Y���Ă�����
	if (m_waitTimer >= MAX_WAIT_TIME)
	{
		//�_�b�V�����Ĉړ�������ς���
		m_moveType = enDash;
	}

	//�G�̉��������v�Z����B
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);
	//�O�̈ړ����������������Y���������v�Z����B
	m_currentMoveDirection = m_currentMoveDirection * (1.0f - m_acceralation) + sideVec * m_acceralation / SPEED_DECAY_RATE;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	//�v���C���[�ƓG�Ƃ̋������v�Z����B
	float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
	//�G����v���C���[�ւ̕������v�Z����B
	Vector3 toTargetVec = targetPos - enemyPos;
	toTargetVec.Normalize();

	//�G���v���C���[��藣�ꂷ������
	if (distance > LIMIT_ENEMY_RECOGNIZE_PLAYER 
		&& (m_moveType != enFrontAndBehind && m_moveType != enAround))
	{
		//�ڋ߂���悤�ɂ���B
		m_moveType = enApproach;
	}

	//�ړ��^�C�v�ɂ���Čv�Z����ړ�������ς���
	switch (m_moveType)
	{
	case enApproach:
		CalcApproachSpeed(toTargetVec, distance);
		break;
	case enFrontAndBehind:
	case enPlayerBehind:
		CalcFrontAndBehindDirection(toTargetVec, distance);
		break;
	case enAround:
		CalcAroundDirection(toTargetVec);
		break;
	case enApproachAndDash:
		CalcApproachAndDashDirection(toTargetVec, distance);
		break;
	case enStay:
		EnemyStayDirection(toTargetVec);
		break;
	case enDash:
		EnemyDashDirection(toTargetVec);
		break;
	default:
		CalcApproachSpeed(toTargetVec, distance);
		break;
	}

	//�ړ����[�h���ς���
	if (m_prevMoveType != m_moveType)
	{
		//���݂̈ړ����[�h���X�V����B
		m_prevMoveType = m_moveType;
		//�����x�����Z�b�g
		m_acceralation = 0.0f;
	}

	//���Ԃŉ����x���グ�Ă���
	m_acceralation += (GameTime().GetFrameDeltaTime() / ADJUST_ACCERALATION_RATE);
	//1�ȏ�ɂ͂Ȃ�Ȃ��悤��
	if (m_acceralation >= 1.0f)
	{
		m_acceralation = 1.0f;
	}

	//�ړ������Ǝ����̃X�s�[�h�����݂̈ʒu�ɉ��Z���A�V�����ʒu���W��Ԃ��B
	return enemyPos + m_currentMoveDirection * m_moveSpeed;
}