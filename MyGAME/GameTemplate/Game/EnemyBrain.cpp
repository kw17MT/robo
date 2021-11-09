#include "stdafx.h"
#include "EnemyBrain.h"

#include "Calculation.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

void EnemyBrain::JudgeMoveType()
{
	//�v���C���[�Ǝ����̋������v�Z����B
	float distance = CalcMethods::CalcDistance(m_enemyPos, m_playerPos);
	//�G�̈ړ����[�h���Ȃɂ��^�[�Q�b�g���Ă��Ȃ���Ԃŋ��������ȉ��Ȃ��
	if (distance < 20000.0f && m_enemyMove.GetEnemyMoveTypes() == enNormalMove)
	{
		//�ڋ߃��[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enApproach);
	}
	//�G�̈ړ����[�h���������Ԃŋ��������ȏ�Ȃ��
	else if(m_enemyMove.GetEnemyMoveTypes() == enRun && distance > 21000.0f)
	{
		//�v���C���[�̎�������邮���郂�[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enAroundPlayer);
	}
	//��郂�[�h�ň�苗���ȉ��ɂȂ��
	else if (m_enemyMove.GetEnemyMoveTypes() == enAroundPlayer && distance < 20000.0f)
	{
		//�ēx�ڋ߃��[�h�ɂ���B
		m_enemyMove.SetEnemyMoveTypes(enApproach);
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