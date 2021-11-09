#include "stdafx.h"
#include "EnemyBrain.h"

#include "Calculation.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float RECOGNITION_DISTANCE = 20000.0f;
	const float LOWEST_ALTITUDE = -10000.0f;
	const float HIGHEST_ALTITUDE = 30000.0f;
}

void EnemyBrain::JudgeMoveType()
{
	//�v���C���[�Ǝ����̋������v�Z����B
	float distance = CalcMethods::CalcDistance(m_enemyPos, m_playerPos);
	//�G�̈ړ����[�h���Ȃɂ��^�[�Q�b�g���Ă��Ȃ���Ԃŋ��������ȉ��Ȃ��
	if (distance < RECOGNITION_DISTANCE && m_enemyMove.GetEnemyMoveTypes() == enNormalMove)
	{
		//�ڋ߃��[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}
	//�G�̈ړ����[�h�������郂�[�h���v���C���[���͂ރ��[�h�ŋ��������ȏ�Ȃ��
	else if((m_enemyMove.GetEnemyMoveTypes() == enSurroundPlayer 
		|| m_enemyMove.GetEnemyMoveTypes() == enRun)
		&& distance > RECOGNITION_DISTANCE)
	{
		//�v���C���[�̎�������邮���郂�[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enAroundPlayer);
	}
	//��郂�[�h�ň�苗���ȉ��ɂȂ��
	else if (m_enemyMove.GetEnemyMoveTypes() == enAroundPlayer && distance < RECOGNITION_DISTANCE)
	{
		//�ēx�ڋ߃��[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}

	//�Œፂ�x�܂œG�������Ă�����
	if (m_enemyPos.y <= LOWEST_ALTITUDE && m_enemyMove.GetEnemyAltitudeState() == enRun)
	{
		//���x�������[�h�ɂ͂���
		m_enemyMove.SetEnemyAltitudeState(enTooLow);
	}
	//�ō��d�x�܂œG�������Ă�����
	else if (m_enemyPos.y >= HIGHEST_ALTITUDE && m_enemyMove.GetEnemyAltitudeState() == enRun)
	{
		//���x�������[�h�ɓ���
		m_enemyMove.SetEnemyAltitudeState(enTooHigh);
	}
}

void EnemyBrain::Work()
{
	//�U�����邩�̔��f��Ɏ����ƃv���C���[�̍��W��^����
	m_enemyAtack.SetEnemyPos(m_enemyPos);
	m_enemyAtack.SetPlayerPos(m_playerPos);

	//���W�ƈړ����[�h���玟�̈ʒu���W���v�Z����B
	m_enemyPos = m_enemyMove.Execute(m_enemyPos, m_playerPos);
	JudgeMoveType();

	m_canShoot = m_enemyAtack.Execute();
}