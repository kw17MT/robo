#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "AmmoGauge.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 10.0f,10.0f,10.0f };
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
	m_skinModelRender->Init("Assets/modelData/Gun/gun3.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	m_ammoGauge = NewGO<AmmoGauge>(0);
	m_ammoGauge->SetAmmoType(enAmmoBullet);
	m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	m_ammoGauge->SetPosition({ -440.0f, -280.0f, 0.0f });

	return true;
}

void MachinGun::Update()
{
	m_shootDelay -= GameTime().GetFrameDeltaTime();

	//to do �X�e�[�g�����g�ŊǗ����鎖
	if (g_pad[0]->IsPress(enButtonRB2) && m_remaining_bullet > 0 && m_shootDelay <= 0.0f)
	{
		//�e�𐶐�
		m_bullets.push_back(NewGO<Bullet>(0));
		//�e�̏������W�n��ݒ�
		m_bullets.back()->SetTargetAndCurrentPos(m_targetPos, m_position);
		m_bullets.back()->SetOwner(enPlayer);
		Quaternion rot;
		rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_targetPos - m_position);
		m_bullets.back()->SetRotation(rot);
		//�c��e����1�}�C�i�X
		m_remaining_bullet--;
		m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
		m_shootDelay = 0.15f;
	}

	if(m_ammoGauge->GetIsFinishedReloaded())
	{
		m_remaining_bullet = MAX_AMMO;
	}

	//���f���̈ʒu���X�V����
	m_skinModelRender->SetPosition(m_position);
}