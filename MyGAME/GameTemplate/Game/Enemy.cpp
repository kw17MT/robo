#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	if (m_player != nullptr) {
		DeleteGO(m_player);
	}
}

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");
	m_skinModelRender = NewGO<SkinModelRender>(0, "enemy");
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr/*"Assets/modelData/enemy/enemy.tks"*/, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});

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