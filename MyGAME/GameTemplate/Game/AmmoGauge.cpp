#include "stdafx.h"
#include "AmmoGauge.h"
#include "SpriteRender.h"
#include "FontRender.h"

namespace
{
	const float BULLET_RELOAD_COMPLETE_TIME = 3.0f;				//�e�̃����[�h����
	const float MISSILE_RELOAD_COMPLETE_TIME = 5.0f;			//�~�T�C���̃����[�h����
	const float RASER_RELOAD_COMPLETE_TIME = 10.0f;				//���[���K���̃����[�h����
	const int GAUGE_SCALE_X = 128;								//�Q�[�W��X�g�嗦
	const int GAUGE_SCALE_Y = 32;								//�Q�[�W��Y�g�嗦
	const float FONT_ADJUST_AMOUNT_X = 50.0f;					//�����̈ʒu�̒����ϐ�
	const float FONT_ADJUST_AMOUNT_Y = 20.0f;					//�����̈ʒu�̒����ϐ�
}

AmmoGauge::~AmmoGauge()
{
	//�摜�̍폜
	DeleteGO(m_spriteRender);
}

bool AmmoGauge::Start()
{
	//�Q�[�W�̐���
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", GAUGE_SCALE_X, GAUGE_SCALE_Y);
	m_spriteRender->SetPivot(m_pivot);

	//�c�e����\��
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"0");

	return true;
}

void AmmoGauge::Reloading(const float reloadTime)
{
	//�����[�h���Ԃɉ����Ċg�嗦���X�V
	m_scale.x = m_reloadTimer / reloadTime;
	//�����[�h�̎��ԂɒB������
	if (m_reloadTimer >= reloadTime)
	{
		//�c�e������
		m_remaining_ammo = m_max_ammo;
		//�^�C�}�[�����Z�b�g
		m_reloadTimer = 0.0f;
		//�����[�h����Ă���
		m_isReloaded = true;
		//�����[�h���I�����
		m_finishReloading = true;
		return;
	}
	//�����[�h��ɉ����Ďc�e�̕\�����𑝉�
	m_remaining_ammo = m_max_ammo * m_scale.x;
}

void AmmoGauge::Update()
{
	//�c�e�𕶎��ɂ���
	std::wstring remainingAmmo = std::to_wstring(m_remaining_ammo);
	m_fontRender->SetText(remainingAmmo.c_str());

	//�c�e��0�ɂȂ��
	if (m_remaining_ammo == 0)
	{
		//�����[�h����Ă��Ȃ���Ԃ�
		m_isReloaded = false;
	}

	//�����[�h����Ă��Ȃ����
	if (!m_isReloaded)
	{
		//�Q�[���^�C����p���ă^�C�}�[
		m_reloadTimer += GameTime().GetFrameDeltaTime();

		//�e�̎�ނɉ����ă����[�h�̒������R���g���[��
		switch (m_ammoType)
		{
		//�}�V���K���̒e�̏ꍇ
		case enAmmoBullet:
			Reloading(BULLET_RELOAD_COMPLETE_TIME);
			break;
		//�~�T�C���̏ꍇ
		case enAmmoMissile:
			Reloading(MISSILE_RELOAD_COMPLETE_TIME);
			break;
		//���[���K���̏ꍇ
		case enAmmoRaser:
			Reloading(RASER_RELOAD_COMPLETE_TIME);
			break;
		}
	}
	//�����[�h����Ă���Ȃ��
	else
	{
		//�����[�h�����̃t���[���ɏI�������
		m_finishReloading = false;
		//�c�e���ɉ������Q�[�W�̒����ɂ���
		m_scale.x = (float)m_remaining_ammo / (float)m_max_ammo;
	}

	//�����̈ʒu���X�V
	m_fontRender->SetPosition({ -m_screenPos.x - FONT_ADJUST_AMOUNT_X, m_screenPos.y + FONT_ADJUST_AMOUNT_Y });
	//�����̊g�嗦���X�V
	m_fontRender->SetScale(m_fontScale);
	//�Q�[�W�摜�̊g�嗦�X�V
	m_spriteRender->SetScale(m_scale);
	//�摜�̈ʒu�X�V
	m_spriteRender->SetPosition(m_screenPos);
}