#include "stdafx.h"
#include "Bullet.h"
#include "SkinModelRender.h"
#include "Enemy.h"

namespace
{
	const int BULLET_SPEED = 1000.0f;
}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	//�e���f���̐���
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);

	return true;
}

Vector3 Bullet::CalcToTargetVec()
{
	//���K�����ꂽ�^�[�Q�b�g�n�_�ւ̃x�N�g�����v�Z����
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Bullet::Update()
{
	//�G�̂܂ł̋�������ړ������Ƒ��x���v�Z����B
	if (m_firstCalc == false)
	{
		m_moveSpeed = CalcToTargetVec() * BULLET_SPEED;
		m_firstCalc = true;
	}

	//���x���l�������ʒu���W��ݒ肷��B
	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);

	//�X�V�����ʒu���W�Ƃ��ׂĂ̓G�Ƃ̋��������߂�
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//�G�ƒe�̋������v�Z����B
		Vector3 diff = enemy->GetPosition() - m_position;
		if (diff.Length() < 200.0f) {
			//�}�V���K���̒e����_���[�W���󂯂����Ƃ�m�点��
			enemy->TakenDamage(enBullet);
			//���S�B
			DeleteGO(this);
			//�I���B
			return false;
		}
		//�N�G���͌p���B
		return true;
		});

	//�e�̎���
	count += GameTime().GetFrameDeltaTime();
	if (count >= 3.0f)
	{
		DeleteGO(this);
	}
}