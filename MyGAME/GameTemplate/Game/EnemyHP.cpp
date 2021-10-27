#include "stdafx.h"
#include "EnemyHP.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_DAMAGE_AMOUNT = 10;			//�}�V���K���̒e�̃_���[�W��
	const int MISSILE_DAMAGE_AMOUNT = 10;			//�~�T�C���̃_���[�W��
	const int RASER_DAMAGE_AMOUNT = 33;				//���[���K���̃_���[�W��

	const float MAX_ENEMY_HP = 100.0f;
}

EnemyHP::~EnemyHP()
{
	//HP�o�[�̃C���X�^���X�j��
	DeleteGO(m_spriteRender);
}

bool EnemyHP::Start()
{
	//HP�o�[�̃��f������
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds",128,32);
	m_spriteRender->SetPivot({ 1.0f,1.5f });

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
		m_isDead = true;
	}
}

void EnemyHP::Update()
{
	//���[���h���W����A�J�����̍s��������ăX�N���[�����W�ɕϊ�����
	Vector2 position;
	g_camera3D->CalcScreenPositionFromWorldPosition(position, m_enemyPos);
	m_screenPos.x = -position.x;
	m_screenPos.y = position.y;

	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//���K��
	enemyToCamera.Normalize();
	//�G�̈ʒu�ƃJ�����̑O�����̓���
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//�G���J�����̑O�����ɂ���Ȃ�Ήf��
	if (dot < 0.0f)
	{
		m_screenPos.z = 0.0f;
	}
	//��둤�ɂ���
	else
	{
		m_screenPos.z = -1.0f;
	}
	m_spriteRender->SetPosition(m_screenPos);

	//�c��HP�ʂɂ����HP�o�[��Z������
	m_scale.x = (float)m_enemyHP / MAX_ENEMY_HP;

	//���̓G�����[���Ƃ���Ă���Ȃ�
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