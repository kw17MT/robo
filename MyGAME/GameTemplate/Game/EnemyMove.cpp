#include "stdafx.h"
#include "EnemyMove.h"

namespace
{
	const float ADJUST_SLOPE_UP_AMOUNT = 0.8f;
	const float  ADJUST_SLOPE_DOWN_AMOUNT = -ADJUST_SLOPE_UP_AMOUNT;
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
	//�v���C���[���͂ރ��[�h��������
	if (m_moveType == enSurroundPlayer)
	{
		//�������������v���C���[��������
		nextPos += toTargetVec * m_moveSpeed / 3.0f;
	}
	else
	{
		//89�x�Ȃ����x�N�g���ɑ��x��K�p���Ď��̈ʒu���W���v�Z����B
		nextPos += toTargetVec * m_moveSpeed;
	}

	return nextPos;
}

Vector3 EnemyMove::CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos)
{
	//�^�[�Q�b�g�ƂȂ�v���C���[�ւ̃x�N�g���Ƃ��̐��K��
	Vector3 toTargetVec = targetPos - enemyPos;
	//�v���C���[�ɋ߂Â���������
	if (toTargetVec.Length() < 1000.0f)
	{
		//�����鏈���ɂ��邩�ǂ���
		srand((unsigned int)time(NULL));
		bool isRun = rand() % 2;
		if (isRun) {
			//�����ɓ]��
			m_moveType = enRun;
		}
		else
		{
			//�v���C���[���͂�ŐϋɓI�ɍU��
			m_moveType = enSurroundPlayer;
		}
	}
	toTargetVec.Normalize();

	//���b�N�I�����Ă����G���|���ꂽ��
	//�i�s���Ă������������݂̃~�T�C���̑O�����ƃ^�[�Q�b�g�ւ̃x�N�g�����ς����߂�B
	if (float dot = toTargetVec.Dot(m_prevMoveDirection) > 0.99f)
	{
		//�ړ�������ۑ�
		m_prevMoveDirection = toTargetVec;
		//���̂܂܌v�Z�������x��K�p�����̂܂܂ł������ɂȂ邽��
		Vector3 nextPos = enemyPos + toTargetVec * m_moveSpeed;

		return nextPos;
	}
	else
	{
		
		//���̂܂܌v�Z�������x��K�p�����̂܂܂ł������ɂȂ邽��
		Vector3 nextPos = enemyPos + toTargetVec * m_moveSpeed;

		
		//�^�[�Q�b�g�ւ̃x�N�g���ƃ~�T�C���̑O�����̏�������v�Z
		Vector3 up = toTargetVec.CalcCross(m_prevMoveDirection);
		Quaternion rot;
		rot.SetRotationDeg(up, dot * 100.0f);
		rot.Apply(nextPos);

		//�ړ�������ۑ�
		m_prevMoveDirection = toTargetVec;

		return nextPos;
	}
}

Vector3 EnemyMove::CalcRunPos(Vector3 enemyPos, Vector3 targetPos)
{
	//�����郂�[�h�ɂȂ�O�̈ړ�������p���đf�ʂ肷��悤�ɓ������B
	return enemyPos + m_prevMoveDirection * m_moveSpeed;
}

Vector3 EnemyMove::AdjustedAltitudePos(Vector3 calcedPos)
{
	switch (m_altitudeState)
	{
	//���[���h�ɑ΂��č��x���������Ă���Ȃ��
	case enTooHigh:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_DOWN_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		calcedPos.y += m_adjustedAltitudeHeight;
		if (calcedPos.y <= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return calcedPos;
		break;
	//���[���h�ɑ΂��č��x���Ⴗ���Ă���ꍇ
	case enTooLow:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_UP_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		calcedPos.y += m_adjustedAltitudeHeight;
		if (calcedPos.y >= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return calcedPos;
		break;
	//����ȏꍇ
	default:
		m_adjustingAltitudeElapsedTime = 0.0f;
		m_adjustedAltitudeHeight /= 1.1f;
		if (abs(m_adjustedAltitudeHeight) < 10.0f)
		{
			m_adjustedAltitudeHeight = 0.0f;
		}
		calcedPos.y += m_adjustedAltitudeHeight;
		return calcedPos;
		break;
	}
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	switch (m_moveType)
	{
	case enNormalMove:
		return AdjustedAltitudePos(CalcNextPos(enemyPos));
		break;
	case enApproach:
		return AdjustedAltitudePos(CalcApproachedPos(enemyPos, targetPos));
		break;
	case enRun:
		return AdjustedAltitudePos(CalcRunPos(enemyPos, targetPos));
		break;
	case enAroundPlayer:
		return AdjustedAltitudePos(CalcNextPos(enemyPos, targetPos));
		break;
	case enSurroundPlayer:
		return AdjustedAltitudePos(CalcNextPos(enemyPos, targetPos));
	default:
		return AdjustedAltitudePos(CalcNextPos(enemyPos));
		break;
	}
}