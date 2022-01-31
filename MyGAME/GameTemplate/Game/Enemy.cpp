#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"
#include "CaptureStateManager.h"
#include "EnemyBrain.h"
#include "EnemyMachinGun.h"
#include "SoundSource.h"
#include "ObjectiveEnemyNum.h"
#include "EnemyRepopManager.h"

namespace
{
	const float SE_VOLUME = 1.0f;
	const Vector3 EXPLOSION_EFFECT_SCALE = { 200.0f,200.0f,200.0f };
	const Vector3 ENEMY_MODEL_SCALE = { 15.0f, 15.0f, 15.0f };
	const Vector3 ENEMY_POP_EFFECT_SCALE = { 300.0f,300.0f,300.0f };
	const Vector3 ENEMY_TRACK_EFFECT_SCALE = { 20.0f,20.0f,20.0f };
}

Enemy::~Enemy()
{
	//�G��HP���O�Ŏ��S���Ă�����
	if (m_enemyHP->IsEnemyDead())
	{
		//�����o��
		CSoundSource* explodeSE = NewGO<CSoundSource>(0);
		explodeSE->Init(L"Assets/sound/explode.wav", false);
		explodeSE->SetVolume(SE_VOLUME);
		explodeSE->Play(false);
	}

	//�ڕW���j���\���C���X�^���X��T����
	ObjectiveEnemyNum* objEne = FindGO<ObjectiveEnemyNum>("objective");
	if (objEne != nullptr)
	{
		//�|���ꂽ�G�̐���1���₷
		objEne->AddKilledEnemyNum();
	}

	//���j�G�t�F�N�g���o��
	Effect effect;
	effect.Init(u"Assets/effect/explosion.efk");
	effect.SetScale(EXPLOSION_EFFECT_SCALE);
	effect.SetPosition(m_position);
	effect.Play();
	effect.Update();

	//���̃C���X�^���X�͎��S���Ă���
	Dead();
	EnemyRepopManager::GetInstance().AddKilledEnemyNum();
	
	DeleteGO(m_machinGun);
	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	DeleteGO(m_enemyHP);
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
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr, enModelUpAxisZ, true);
	m_skinModelRender->SetScale(ENEMY_MODEL_SCALE);

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
	const float moveSpeed = rand() % 20 + 10.0f;
	m_enemyBrain.SetMoveSpeed(moveSpeed);

	m_machinGun = NewGO<EnemyMachinGun>(0);

	Effect effect;
	effect.Init(u"Assets/effect/popEnemy.efk");
	effect.SetScale(ENEMY_POP_EFFECT_SCALE);
	effect.SetPosition(m_position);
	effect.Play();
	effect.Update();

	//���f���̈ʒu��ݒ�
	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	// ���W�֌W //////////////////////////////////////////////////////////////////////////////////////
	m_enemyBrain.MemoryPlayerPos(m_player->GetRoboPosition());
	m_enemyBrain.MemoryEnemyPos(m_position);
	m_enemyBrain.Work();
	//�v�Z�������̈ʒu���W���擾
	m_position = m_enemyBrain.GetNextEnemyPos();
	//�ʒu���X�V
	m_skinModelRender->SetPosition(m_position);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// UI�֌W ////////////////////////////////////////////////////////////////////////////////////////
	//�v���C���[�Ǝ����i�G�j�̋������v�����A�����ɂ����̏���ۑ�
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetRoboPosition());
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

	m_machinGun->SetPosition(m_position);
	if (m_enemyBrain.JudgeCanShoot())
	{
		m_machinGun->SetTargetPos(m_player->GetRoboPosition());
		m_machinGun->SetCanShoot(true);
	}

	Effect effect;
	effect.Init(u"Assets/effect/enemyTrack.efk");
	effect.SetPosition(m_position);
	effect.SetScale(ENEMY_TRACK_EFFECT_SCALE);
	effect.Play();
	effect.Update();

	//HP���Ȃ��Ȃ�����
	if (m_enemyHP->IsEnemyDead())
	{
		//����
		DeleteGO(this);
	}

	m_skinModelRender->SetRotation(m_enemyBrain.GetEnemyRotation());
}