#include "stdafx.h"
#include "PlayerHP.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "SoundSource.h"

namespace
{
	const float BULLET_DAMAGE = 1.0f;			//�e�ۂ̃_���[�W��
	const float MISSILE_DAMAGE = 10.0f;			//�~�T�C���̃_���[�W��
	const float RASER_DAMAGE = 20.0f;			//���[���K���̃_���[�W��
	const float MAX_HP = 100.0f;				//�ő�HP 
	const float FONT_SIZE = 0.5f;				//�����T�C�Y
	const float DECREASE_RATE = 3.0f;
}

PlayerHP::~PlayerHP()
{
	//�摜�C���X�^���X�̍폜
	DeleteGO(m_spriteRender);
}

void PlayerHP::ApplyDamage(EnPlayerDamageTypes enemyDamageType)
{
	switch (enemyDamageType)
	{
	//�e�ۂɍU�����ꂽ��
	case enEnemyBullet:
		//�e�ۃ_���[�W�K�p
		m_playerHp -= BULLET_DAMAGE;
		break;
	//�~�T�C���ɍU�����ꂽ��
	case enEnemyMissile:
		//�~�T�C���_���[�W�K�p
		m_playerHp -= MISSILE_DAMAGE;
		break;
	//���[���K���ɍU�����ꂽ��
	case enEnemyRaser:
		//���[���K���_���[�W�K�p
		m_playerHp -= RASER_DAMAGE;
		break;
	}
}

bool PlayerHP::Start()
{
	//�摜�C���X�^���X����
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/HP.dds", 500, 100);
	//�摜�̈ʒu���Œ�
	m_spriteRender->SetPosition(m_screenPos);

	////�����C���X�^���X����
	//m_fontRender = NewGO<FontRender>(0);
	////�󏉊���
	//m_fontRender->SetText(L"");
	////�傫���ݒ�
	//m_fontRender->SetScale(FONT_SIZE);
	////�ʒu�ݒ�
	//m_fontRender->SetPosition({ m_screenPos.x, m_screenPos.y });
	return true;
}

void PlayerHP::Update()
{
	//�v���C���[�̎c��HP�𕶎���ɕϊ�
	//std::wstring playerHp = std::to_wstring((int)m_playerHp);
	//m_fontRender->SetText(playerHp.c_str());

	//HP���Ȃ��Ȃ�����
	if (m_playerHp <= 0.0f)
	{
		m_playerHp = 0;
		//�v���C���[�͐����Ă��Ȃ�
		m_isPlayerAlive = false;
	}

	//���ۂ�HP�̒l�ƁAHP�Q�[�W�̃T�C�Y���Ⴄ�Ƃ�
	if (m_playerHp != m_hpSize)
	{
		//���ۂ�HP��HP�o�[�̍����v�Z����
		float def = m_hpSize - m_playerHp;
		//���̍���萔�Ŋ��菙�X�ɍ��𖄂߂Ă���
		m_hpSize -= def / DECREASE_RATE;
		//���ۂ�HP���A�Q�[�W�̃T�C�Y���������Ȃ��悤�ɒ���
		if (m_playerHp >= m_hpSize)
		{
			m_hpSize = m_playerHp;
		}
	}

	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= 0.7f
		&& !m_isSoundBuzzer)
	{
		CSoundSource* buzzerSE = NewGO<CSoundSource>(0);
		buzzerSE->Init(L"Assets/sound/HpBuzzer.wav", false);
		buzzerSE->SetVolume(0.5f);
		buzzerSE->Play(false);

		m_isSoundBuzzer = true;
	}
	m_spriteRender->SetSpriteSizeRate(HpSize);

	//�c��HP�ʂɏ]���ĉ摜�̑傫����ύX����
	//m_spriteRender->SetSpriteSizeRate(1.0f - m_playerHp / MAX_HP);
}