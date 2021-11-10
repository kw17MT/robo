#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "AmmoGauge.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 0.2f,0.2f,0.2f };
	const int MAX_AMMO = 100;
}

MachinGun::~MachinGun()
{
	//���ݐ����Ă���e�̍폜
	for (auto i : m_bullets)
	{
		DeleteGO(m_bullets.back());
		m_bullets.pop_back();
	}
	m_bullets.clear();
	//���f���̍폜
	DeleteGO(m_skinModelRender);

	DeleteGO(m_ammoGauge);
}

bool MachinGun::Start()
{
	//�}�V���K���̃��f������
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	m_ammoGauge = NewGO<AmmoGauge>(0);
	m_ammoGauge->SetAmmoType(enAmmoBullet);
	m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	m_ammoGauge->SetPosition({ -400.0f, -300.0f, 0.0f });

	return true;
}

void MachinGun::Update()
{
	//to do �X�e�[�g�����g�ŊǗ����鎖
	if (g_pad[0]->IsPress(enButtonRB2) && m_remaining_bullet > 0)
	{
		//�e�𐶐�
		m_bullets.push_back(NewGO<Bullet>(0));
		//�e�̏������W�n��ݒ�
		m_bullets.back()->SetTargetAndCurrentPos(m_targetPos, m_position);
		//�c��e����1�}�C�i�X
		m_remaining_bullet--;
		m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	}

	if(m_ammoGauge->IsReloaded())
	{
		m_remaining_bullet = MAX_AMMO;
	}

	//���f���̈ʒu���X�V����
	m_skinModelRender->SetPosition(m_position);
}