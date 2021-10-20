#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");
	m_skinModelRender = NewGO<SkinModelRender>(0, "enemy");
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr/*"Assets/modelData/enemy/enemy.tks"*/, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});
	m_position = { 0.0f,400.0f,-5000.0f };
	//m_skinModelRender->InitCharaCon(100.0f, 100.0f, m_position);

	m_displayDistance = NewGO<DisplayDistance>(0);
	m_displayDistance->SetEnemyPos(m_position);

	m_enemyStateIcon = NewGO<EnemyStateIcon>(0);
	m_enemyStateIcon->SetEnemyPos(m_position);
	m_enemyStateIcon->JudgeState(m_distance);

	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	//�v���C���[�Ǝ����i�G�j�̋������v�����A�����ɂ����̏���ۑ�
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
	//�����ɂ��Ă��郌�e�B�N���Ɉʒu���W��^����
	m_enemyStateIcon->SetEnemyPos(m_position);
	//�v���C���[�Ǝ����̋�����^���ă��e�B�N���̏�Ԃ��X�V����B
	m_enemyStateIcon->JudgeState(m_distance);
	//�ʒu���X�V
	m_skinModelRender->SetPosition(m_position);
}