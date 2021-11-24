#include "stdafx.h"
#include "PlayerEN.h"
#include "SpriteRender.h"

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
			}
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
		if (m_playerEN >= 100.0f)
		{
			m_playerEN = 100.0f;
		}
	}

	//�G�l���M�[�����ׂĎg���؂�����
	if (m_playerEN <= 0.0f)
	{
		m_playerEN = 0.0f;
		//�G�l���M�[���Ȃ�
		m_isPlayerEnRemain = false;
	}
	//�񕜂���������
	if (m_playerEN >= MAX_EN)
	{
		//�G�l���M�[������
		m_isPlayerEnRemain = true;
	}

	//�G�l���M�[�̎c�ʂɏ]���ĉ摜�̑傫����ݒ肷��B
	m_spriteRender->SetSpriteSizeRate(1.0f - m_playerEN / MAX_EN);
}