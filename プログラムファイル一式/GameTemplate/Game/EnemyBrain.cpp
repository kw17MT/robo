#include "stdafx.h"
#include "EnemyBrain.h"
#include "GameDirector.h"

void EnemyBrain::Work()
{
	//���W�ƈړ����[�h���玟�̈ʒu���W���v�Z����B
	m_enemyPos = m_enemyMove.Execute(m_enemyPos, m_playerPos);
	//�ړ������ɓG�̃��f���̐��ʂ������悤�ȉ�]���v�Z����B
	m_enemyRotation.SetMoveDirection(m_enemyMove.GetEnemyMoveDireciton(), m_playerPos - m_enemyPos);
	m_rot = m_enemyRotation.GetCalcedRotation();

	//�Q�[���������e���o���悤�ɂ���
	if (GameDirector::GetInstance().GetGameScene() == enInGame)
	{
		m_canShoot = m_enemyAtack.Execute();
	}
}