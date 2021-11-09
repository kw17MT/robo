#include "stdafx.h"
#include "EnemyMove.h"

namespace
{
	const float MOVE_SPEED = 30.0f;
}

Vector3 EnemyMove::CalcNextPos(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 nextPos = enemyPos;
	//���S�Ƃ���^�[�Q�b�g�܂ł̃x�N�g�������߂�
	Vector3 toTargetVec = targetPos - enemyPos;
	//�ʏ�ړ����͍��x�͉����Ȃ�
	toTargetVec.y = 0.0f;
	//�ړ����x�ɗ��p���邽�ߐ��K�����s��
	toTargetVec.Normalize();

	Quaternion qRot;
	//Y�������89�x�Ȃ���
	qRot.SetRotationDegY(89.0f);
	//���_�܂ł̃x�N�g����89�x�Ȃ���
	qRot.Apply(toTargetVec);
	//���̃t���[���̈ړ��������L�^
	m_prevMoveDirection = toTargetVec;
	//89�x�Ȃ����x�N�g���ɑ��x��K�p���Ď��̈ʒu���W���v�Z����B
	nextPos += toTargetVec * MOVE_SPEED;

	return nextPos;
}

Vector3 EnemyMove::CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos)
{
	//�^�[�Q�b�g�ƂȂ�v���C���[�ւ̃x�N�g���Ƃ��̐��K��
	Vector3 toTargetVec = targetPos - enemyPos;
	//�v���C���[�ɋ߂Â���������
	if (toTargetVec.Length() < 500.0f)
	{
		//�����ɓ]��
		m_moveType = enRun;
	}
	toTargetVec.Normalize();

	/////////////////////////////////////////////////////////////////////////
	//���b�N�I�����Ă����G���|���ꂽ��
	// 
		//�i�s���Ă������������݂̃~�T�C���̑O�����ƃ^�[�Q�b�g�ւ̃x�N�g�����ς����߂�B
	if (float dot = toTargetVec.Dot(m_prevMoveDirection) > 0.99f)
	{
		//�ړ�������ۑ�
		m_prevMoveDirection = toTargetVec;
		//���̂܂܌v�Z�������x��K�p�����̂܂܂ł������ɂȂ邽��
		Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;

		return nextPos;
	}
	else
	{
		
		//���̂܂܌v�Z�������x��K�p�����̂܂܂ł������ɂȂ邽��
		Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;

		
		//�^�[�Q�b�g�ւ̃x�N�g���ƃ~�T�C���̑O�����̏�������v�Z
		Vector3 up = toTargetVec.CalcCross(m_prevMoveDirection);
		Quaternion rot;
		rot.SetRotationDeg(up, dot * 100.0f);
		rot.Apply(nextPos);

		//�ړ�������ۑ�
		m_prevMoveDirection = toTargetVec;

		return nextPos;
	}
	


	/////////////////////////////////////////////////////////////////////////



	////�ړ�������ۑ�
	//m_prevMoveDirection = toTargetVec;
	////���̂܂܌v�Z�������x��K�p�����̂܂܂ł������ɂȂ邽��
	//Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;
	//
	//return nextPos;
}

Vector3 EnemyMove::CalcRunPos(Vector3 enemyPos, Vector3 targetPos)
{
	//�^�[�Q�b�g�ƂȂ�v���C���[�ւ̃x�N�g���Ƃ��̐��K��
	//Vector3 toTargetVec = targetPos - enemyPos;
	//toTargetVec.Normalize();

	//�����邽�߂̏������]�����v�Z
	//Vector3 up;
	//up = toTargetVec.CalcCross(m_prevMoveDirection);

	//�ړ�������ۑ�
	//m_prevMoveDirection = toTargetVec;

	//Quaternion qRot;
	//180�x��]�����ē�����������쐬����B
	//qRot.SetRotation(toTargetVec, 180.0f);
	//qRot.Apply(toTargetVec);

	//�����郂�[�h�ɂȂ�O�̈ړ�������p���đf�ʂ肷��悤�ɓ������B
	return enemyPos + m_prevMoveDirection/*toTargetVec*/ * MOVE_SPEED;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 nextPos;
	switch (m_moveType)
	{
	case enNormalMove:
		return CalcNextPos(enemyPos);
		break;
	case enApproach:
		return CalcApproachedPos(enemyPos, targetPos);
		break;
	case enRun:
		return CalcRunPos(enemyPos, targetPos);
		break;
	case enAroundPlayer:
		return CalcNextPos(enemyPos, targetPos);
		break;
	default:
		return CalcNextPos(enemyPos);
		break;
	}
}