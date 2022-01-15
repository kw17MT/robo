#include "stdafx.h"
#include "PlayerEN.h"
#include "SpriteRender.h"
#include "SoundSource.h"

namespace
{
	const float MAX_EN = 100.0f;				//�ő�G�l���M�[��
	const float EN_REDUCE_AMOUNT = 0.5f;
}

PlayerEN::~PlayerEN()
{
	//EN�\���摜�̍폜
	DeleteGO(m_spriteRender);
}

bool PlayerEN::Start()
{
	//�摜�C���X�^���X�̐���
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/EN.dds", 500, 100);
	//�ʒu�ݒ�
	m_spriteRender->SetPosition(m_screenPos);

	return true;
}

void PlayerEN::Update()
{
	//RB1�{�^���������ꂽ��
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//�G�l���M�[���c���Ă���Ȃ��
		if (m_isPlayerEnRemain) {
			//�萔������
			m_playerEN -= EN_REDUCE_AMOUNT;
			if (m_playerEN <= 0.0f)
			{
				m_playerEN = 0.0f;
				//�G�l���M�[���Ȃ�
				m_isPlayerEnRemain = false;

				CSoundSource* downSE = NewGO<CSoundSource>(0);
				downSE->Init(L"Assets/sound/EnDown.wav", false);
				downSE->SetVolume(0.5f);
				downSE->Play(false);
			}

			m_blinkingTime = 0.0f;
		}
		//��x���ׂĂ�EN���g���؂��Ă���Ȃ�΁i�I�[�o�[�q�[�g���j
		else
		{
			//�萔����
			m_playerEN += 0.5f;
			if (m_playerEN >= 100.0f)
			{
				m_playerEN = 100.0f;
			}
		}
	}
	//RB1�{�^���͉�����Ă��Ȃ�
	else
	{
		//�萔����
		m_playerEN += 0.5f;
		if (m_playerEN >= MAX_EN)
		{
			m_playerEN = MAX_EN;
		}
	}

	//�񕜂���������
	if (m_playerEN >= MAX_EN)
	{
		//�G�l���M�[������
		m_isPlayerEnRemain = true;
	}

	//�G�l���M�[�̎c�ʂɏ]���ĉ摜�̑傫����ݒ肷��B
	m_spriteRender->SetSpriteSizeRate(1.0f - m_playerEN / MAX_EN);

	//EN���Ȃ��Ȃ��ăI�[�o�[�q�[�g���Ă��鎞
	if (!m_isPlayerEnRemain)
	{
		//�_�ł��鎞�Ԃ��f���^�^�C���ő��₷
		m_blinkingTime += GameTime().GetFrameDeltaTime();
		if (m_blinkingTime >= 0.4f)
		{
			//���ɖ߂�
			m_blinkingTime = 0.0f;
		}
		//EN�Q�[�W�����ď�������i1�T�C�N��������j
		if (m_blinkingTime >= 0.2f)
		{
			//�_�ł����邽�߂ɃQ�[�W��S���\�������Ȃ��悤�ɂ���
			m_spriteRender->SetSpriteSizeRate(1.0f);
		}
	}
}