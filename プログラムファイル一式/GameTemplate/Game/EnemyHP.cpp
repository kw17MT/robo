#include "stdafx.h"
#include "EnemyHP.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_DAMAGE_AMOUNT = 10;				//�}�V���K���̒e�̃_���[�W��
	const int MISSILE_DAMAGE_AMOUNT = 100;				//�~�T�C���̃_���[�W��
	const int RASER_DAMAGE_AMOUNT = 33;					//���[���K���̃_���[�W��

	const float MAX_ENEMY_HP = 100.0f;					//�G�̍ő�HP
	const int ENEMY_HPBAR_X = 128;						//HP�o�[�̉���
	const int ENEMY_HPBAR_Y = 32;						//HP�o�[�̏c��
	const Vector2 ENEMY_HPBAR_PIVOT = { 1.0f, 1.5f };	//HP�o�[�̊�_�i�摜�̊֌W�シ����������_��傫���Ƃ����j
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

EnemyHP::~EnemyHP()
{
	//HP�o�[�̃C���X�^���X�j��
	DeleteGO(m_spriteRender);
}

bool EnemyHP::Start()
{
	//HP�o�[�̃��f������
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", ENEMY_HPBAR_X, ENEMY_HPBAR_Y);
	m_spriteRender->SetPivot(ENEMY_HPBAR_PIVOT);

	return true;
}

void EnemyHP::ApplyDamage(EnDamageTypes damageType)
{
	//�_���[�W�̃^�C�v�ɂ���Ďc��HP���v�Z����
	switch (damageType)
	{
	case enBullet:
		m_enemyHP -= BULLET_DAMAGE_AMOUNT;
		break;
	case enMissile:
		m_enemyHP -= MISSILE_DAMAGE_AMOUNT;
		break;
	case enRaser:
		m_enemyHP -= RASER_DAMAGE_AMOUNT;
		break;
	}

	//HP��0�ȉ��ɂȂ��
	if (m_enemyHP <= 0.0f)
	{
		//�|���ꂽ���Ƃ��L�^
		m_isEnemyDead = true;
	}
}

void EnemyHP::Update()
{
	//���[���h���W����A�J�����̍s��������ăX�N���[�����W�ɕϊ�����
	CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	m_spriteRender->SetPosition(m_screenPos);

	//�c��HP�ʂɂ����HP�o�[��Z������
	m_scale.x = (float)m_enemyHP / MAX_ENEMY_HP;

	//���̓G���^�[�Q�b�g����Ă���Ȃ�
	if (m_isTargeted) {
		//�c��HP�ʂɂ�����HP�o�[�̒�����
		m_spriteRender->SetScale(m_scale);
	}
	else
	{
		//�^�[�Q�b�g����Ă��Ȃ��Ƃ��͉B��
		m_spriteRender->SetScale(Vector3::Zero);
	}
}