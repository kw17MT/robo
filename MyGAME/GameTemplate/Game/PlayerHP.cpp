#include "stdafx.h"
#include "PlayerHP.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "SoundSource.h"

namespace
{
	const float BULLET_DAMAGE = 3.0f;			//�e�ۂ̃_���[�W��
	const float MISSILE_DAMAGE = 10.0f;			//�~�T�C���̃_���[�W��
	const float RASER_DAMAGE = 20.0f;			//���[���K���̃_���[�W��
	const float MAX_HP = 100.0f;				//�ő�HP 
	const float DECREASE_RATE = 3.0f;
	const float SE_VOLUME = 0.4f;
	const float HPBAR_DANGER_SIZE = 0.7f;
	const float DISAPPEAR_RATE = 0.01f;
}

PlayerHP::~PlayerHP()
{
	//�摜�C���X�^���X�̍폜
	DeleteGO(m_spriteRender);
	DeleteGO(m_damagedSprite);
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
	m_damagedSpriteAlpha = 0.7f;
	SoundDamagedSE(rand() % 2);
}

bool PlayerHP::Start()
{
	//�摜�C���X�^���X����
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/HP.dds", 500, 100);
	//�摜�̈ʒu���Œ�
	m_spriteRender->SetPosition(m_screenPos);

	m_damagedSprite = NewGO<SpriteRender>(0);
	m_damagedSprite->Init("Assets/Image/Damaged/damaged.dds", 1280, 720);
	m_damagedSprite->SetScale(Vector3::One);
	m_damagedSprite->SetAlpha(m_damagedSpriteAlpha);

	return true;
}

void PlayerHP::SoundDamagedSE(int soundNo)
{
	CSoundSource* damaged = NewGO<CSoundSource>(0);
	switch (soundNo)
	{
	case 0:
		damaged->Init(L"Assets/sound/damaged.wav", false);

		break;
	case 1:
		damaged->Init(L"Assets/sound/damaged1.wav", false);

		break;
	}
	damaged->SetVolume(SE_VOLUME);
	damaged->Play(false);
}

float PlayerHP::GetHPBarAmountInDanger()
{
	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= HPBAR_DANGER_SIZE)
	{
		//�c��HP�o�[�͊댯��Ԃ�HP�o�[�̉�����
		float remainAmount = 1.0f - (1.0f - HpSize) / (1.0f - HPBAR_DANGER_SIZE);
		return remainAmount;
	}
	else
	{
		return 0.0f;
	}
}

void PlayerHP::Update()
{
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

	//�c��HP����Q�[�W�̑傫�����v�Z����
	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= HPBAR_DANGER_SIZE)
	{
		//�u�U�[��炵�Ă��Ȃ�������
		if (!m_isSoundBuzzer)
		{
			//����炷
			CSoundSource* buzzerSE = NewGO<CSoundSource>(0);
			buzzerSE->Init(L"Assets/sound/HpBuzzer.wav", false);
			buzzerSE->SetVolume(SE_VOLUME);
			buzzerSE->Play(false);

			m_isSoundBuzzer = true;
		}

		////�c��HP�o�[�͊댯��Ԃ�HP�o�[�̉�����
		//float remainAmount = 1.0f - (1.0f - HpSize) / (1.0f - HPBAR_DANGER_SIZE);
		////�ݒ肳��Ă��郂�m�N�������A����v�Z�����c�芄�����傫�����
		//if (remainAmount > RenderingEngine::GetInstance()->GetMonochromeRate())
		//{
		//	//���m�N�������Z�b�g
		//	RenderingEngine::GetInstance()->SetMonochromeRate(remainAmount);
		//}
	}

	m_damagedSpriteAlpha -= DISAPPEAR_RATE;
	if (m_damagedSpriteAlpha <= 0.0f)
	{
		m_damagedSpriteAlpha = 0.0f;
	}
	m_damagedSprite->SetAlpha(m_damagedSpriteAlpha);
	m_spriteRender->SetSpriteSizeRate(HpSize);
}