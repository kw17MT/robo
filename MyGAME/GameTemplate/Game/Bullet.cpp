#include "stdafx.h"
#include "Bullet.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const int BULLET_SPEED = 100.0f;
}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_effect);
}

bool Bullet::Start()
{
	//�e���f���̐���
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->Init("Assets/modelData/bullet/a.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);


	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/bulletTail2.efk");
	m_effect->SetScale({ 5.0f,5.0f,10.0f });

	m_effect->Update();
	return true;
}

Vector3 Bullet::CalcToTargetVec()
{
	//���K�����ꂽ�^�[�Q�b�g�n�_�ւ̃x�N�g�����v�Z����
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Bullet::BehaveChangedByOwner()
{
	switch (m_owner)
	{
	case enPlayer:
		//�X�V�����ʒu���W�Ƃ��ׂĂ̓G�Ƃ̋��������߂�
		QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
			//�G�ƒe�̋������v�Z����B
			Vector3 diff = enemy->GetPosition() - m_position;
			if (diff.Length() < 200.0f) {
				if (!enemy->IsDead()) {
					//�}�V���K���̒e����_���[�W���󂯂����Ƃ�m�点��
					enemy->TakenDamage(enBullet);
				}
				//���S�B
				DeleteGO(this);
				//�I���B
				return false;
			}
			//�N�G���͌p���B
			return true;
			});
		break;

	case enEnemy:
		//�X�V�����ʒu���W�Ƃ��ׂĂ̓G�Ƃ̋��������߂�
		QueryGOs<Player>("player", [&](Player* player) {
			//�G�ƒe�̋������v�Z����B
			Vector3 diff = player->GetPosition() - m_position;
			if (diff.Length() < 200.0f) {
				//�}�V���K���̒e����_���[�W���󂯂����Ƃ�m�点��
				player->TakenDamage(enEnemyBullet);
				//���S�B
				DeleteGO(this);
				//�I���B
				return false;
			}
			//�N�G���͌p���B
			return true;
			});
		break;
	}
	
}

void Bullet::SetRotation(Quaternion rot)
{ 
	m_rot = rot;
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
	m_skinModelRender->SetRotation(m_rot);

	BehaveChangedByOwner();

	m_effect->SetPosition(m_position);
	m_effect->SetRotation(m_rot);
	m_effect->Play();
	//�e�̎���
	count += GameTime().GetFrameDeltaTime();
	if (count >= 3.0f)
	{
		DeleteGO(this);
	}
}