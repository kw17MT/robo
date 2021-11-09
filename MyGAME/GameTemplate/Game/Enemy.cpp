#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"
#include "CaptureStateManager.h"
#include "EnemyBrain.h"
#include "EnemyMachinGun.h"

Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	DeleteGO(m_enemyHP);

	//�G���|�ꂽ���߁A���b�N�I����Ԃ������Ȃ��ɂ��邩�A���̓G�����b�N�I�����邩���肷��B
	CaptureStateManager::GetInstance().SetCaptureState(None, false);
}

void Enemy::TakenDamage(EnDamageTypes damageType)
{
	//HP�Ƀ_���[�W�𔽉f����B
	m_enemyHP->ApplyDamage(damageType);
}

bool Enemy::Start()
{
	//�v���C���[�C���X�^���X�̏��擾
	m_player = FindGO<Player>("player");

	//�G�̃��f�����C���X�^���X��
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr/*"Assets/modelData/enemy/enemy.tks"*/, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});

	//�����i�����j��\��
	m_displayDistance = NewGO<DisplayDistance>(0);
	//�ʒu��G�ɐݒ�
	m_displayDistance->SetEnemyPos(m_position);

	//�G�ɂ�����A�C�R����\��
	m_enemyStateIcon = NewGO<EnemyStateIcon>(0);
	//�ʒu��G�ɐݒ�
	m_enemyStateIcon->SetEnemyPos(m_position);
	//�v���C���[�ƓG�̋�����n��
	m_enemyStateIcon->JudgeState(m_distance);

	m_enemyStateIcon->SetReticleInstance(m_player->GetReticleInstance());

	//HP�o�[��\��
	m_enemyHP = NewGO<EnemyHP>(0);
	//�ʒu��G�ɐݒ�
	m_enemyHP->SetEnemyPos(m_position);

	//20�`30�̃����_���Ȑ��l���쐬�B
	const float moveSpeed = rand() % 10 + 20.0f;
	m_enemyBrain.SetMoveSpeed(moveSpeed);

	m_machinGun = NewGO<EnemyMachinGun>(0);

	//���f���̈ʒu��ݒ�
	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	// ���W�֌W //////////////////////////////////////////////////////////////////////////////////////
	m_enemyBrain.MemoryPlayerPos(m_player->GetPosition());
	m_enemyBrain.MemoryEnemyPos(m_position);
	m_enemyBrain.Work();
	//�v�Z�������̈ʒu���W���擾
	m_position = m_enemyBrain.GetNextEnemyPos();
	//�ʒu���X�V
	m_skinModelRender->SetPosition(m_position);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// UI�֌W ////////////////////////////////////////////////////////////////////////////////////////
	//�v���C���[�Ǝ����i�G�j�̋������v�����A�����ɂ����̏���ۑ�
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
	//�����ɂ��Ă��郌�e�B�N���Ɉʒu���W��^����
	m_enemyStateIcon->SetEnemyPos(m_position);
	//�v���C���[�Ǝ����̋�����^���ă��e�B�N���̏�Ԃ��X�V����B
	m_enemyStateIcon->JudgeState(m_distance);
	//�������^�[�Q�b�g����Ă�����HP�o�[���o��������B
	m_enemyHP->IsEnemyTargeted(m_enemyStateIcon->IsTargeted());
	//HP�o�[�̈ʒu���X�V
	m_enemyHP->SetEnemyPos(m_position);
	//�����̃C���X�^���X���A�C�R���ɓn���Ă��
	m_enemyStateIcon->SetEnemyObject(this);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_enemyBrain.JudgeCanShoot())
	{
		m_machinGun->SetTargetAndCurrentPos(m_player->GetPosition(), m_position);
		m_machinGun->SetCanShoot(true);
	}

	//HP���Ȃ��Ȃ�����
	if (m_enemyHP->IsDead())
	{
		//����
		DeleteGO(this);
	}
}