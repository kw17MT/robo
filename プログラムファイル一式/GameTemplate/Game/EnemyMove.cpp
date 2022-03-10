#include "stdafx.h"
#include "EnemyMove.h"

#include "EnemyApproachState.h"
#include "EnemyDashState.h"
#include "EnemyAroundState.h"
#include "EnemyStayState.h"
#include "EnemyBehindState.h"
#include "EnemyBackState.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float ADJUST_ACCERALATION_RATE = 5.0f;						//�����x�̑��������p
	const int MAX_ROTATION_DEGREE = 60;									//��]����ő��
	const float SPEED_DECAY_RATE = 10.0f;								//���x�̌�����
}

void EnemyMove::CalcApproachDirection(Vector3 toTargetVec)
{
	//�v���C���[�Ɍ������ړ�����
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);
}

void EnemyMove::CalcBackDirection(Vector3 toTargetVec)
{
	//�v���C���[�Ɣ��Ε����Ɍ������x�N�g��
	m_currentMoveDirection = (toTargetVec * -1.0f) * m_acceralation + (toTargetVec * (1.0f - m_acceralation));
}

void EnemyMove::CalcAroundDirection(Vector3 toTargetVec)
{
	//�v���C���[�ɑ΂��鉡�������Z�o
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);

	//��ނ��Ȃ��牡�Ɉړ����鐳�K���ړ��������v�Z����B
	toTargetVec += sideVec;
	toTargetVec *= -1.0f;
	toTargetVec.Normalize();

	//�����x�ɉ����ēK�p�ړ����������߂�
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);;
}

void EnemyMove::CalcDashDirection(Vector3 toTargetVec)
{
	//�ŏ��̃_�b�V�������̌v�Z���I����Ă��Ȃ����
	if (!m_isCalcEnemyDashDirection)
	{
		//�������x�N�g���̌v�Z
		Vector3 axisSide = toTargetVec.CalcCross(g_vec3AxisY);
		//������x�N�g���̌v�Z
		Vector3 axisUp = toTargetVec.CalcCross(axisSide);

		//1~60�̐��l�𓾂�
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
}

void EnemyMove::CalcStayDirection(Vector3 toTargetVec)
{
	//�G�̉��������v�Z����B
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);
	//�O�̈ړ����������������Y���������v�Z����B
	m_currentMoveDirection = m_currentMoveDirection * (1.0f - m_acceralation) + sideVec * m_acceralation / SPEED_DECAY_RATE;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	//�G����v���C���[�ւ̕������v�Z����B
	Vector3 toTargetVec = targetPos - enemyPos;
	toTargetVec.Normalize();

	//�v���C���[�ƓG�Ƃ̋������v�Z����B
	m_param.distanceBetweenEnemyToPlayer = CalcMethods::CalcDistance(enemyPos, targetPos);;
	m_param.toTargetDirection = toTargetVec;

	//���݂̈ړ���Ԃ����Ԃ�v���C���[�Ƃ̋����ɉ����čX�V����B
	EnemyMoveState* currentState = m_state->Update(m_param);
	//�X�V�\��̏�Ԃƌ��݂̏�Ԃ�����Ă�����
	if (currentState != m_state)
	{
		//��Ԃ��X�V
		m_state = currentState;
		//�^�C�}�[�Ȃǂ̃p�����[�^��������
		m_state->Init(m_param);
		//�����𖳂���
		m_acceralation = 0.0f;
		//�_�b�V���������v�Z���Ȃ���
		m_isCalcEnemyDashDirection = false;
	}

	if (m_state == EnemyApproachState::GetInstance())
	{
		CalcApproachDirection(toTargetVec);
	}
	else if(m_state == EnemyDashState::GetInstance())
	{
		CalcDashDirection(toTargetVec);
	}
	else if (m_state == EnemyAroundState::GetInstance())
	{
		CalcAroundDirection(toTargetVec);
	}
	else if (m_state == EnemyBehindState::GetInstance())
	{
		CalcDashDirection(toTargetVec);
	}
	else if (m_state == EnemyBackState::GetInstance())
	{
		CalcBackDirection(toTargetVec);
	}
	else
	{
		CalcStayDirection(toTargetVec);
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