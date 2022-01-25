#include "stdafx.h"
#include "AfterGameOverScene.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

namespace
{
	const Vector4 BLACK = { 0.0f,0.0f,0.0f,0.0f };
	const float SE_VOLUME = 0.5f;
	const float BACKGROUND_DARKNESS = 0.7f;
}

AfterGameOverScene::~AfterGameOverScene()
{
	//�I�����̉摜���폜����
	for (int i = 0; i < enSpriteNum; i++)
	{
		DeleteGO(m_sprite[i]);
	}
	DeleteGO(m_blackSprite);
}

bool AfterGameOverScene::Start()
{
	//�I�����̉摜������
	for (int i = 0; i < enSpriteNum; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(10);
	}
	//Replay�摜�ŏ�����
	m_sprite[enRePlayButton]->Init("Assets/image/GameOver/PlayAgain.dds", 400, 130, "PSWarningSign");
	//ToTitle�摜�ŏ�����
	m_sprite[enToTitleButton]->Init("Assets/image/GameOver/ReturnToTitle.dds", 400, 130, "PSWarningSign");
	//�摜�̈ʒu��������
	m_sprite[enRePlayButton]->SetPosition({ 0.0f,100.0f,0.0f });
	m_sprite[enToTitleButton]->SetPosition({ 0.0f,-100.0f,0.0f });
	


	return true;
}

void AfterGameOverScene::SelectButton()
{
	//���̃V�[���ɑJ�ڂ��Ȃ��Ƃ������A�{�^������ł���悤�ɂ���
	if (m_canSelectButton)
	{
		//��{�^����������Ă���
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			//���ݑI��ł���摜�̎�ނɉ����Ĕ�������
			switch (m_selectedSpriteType)
			{
			case enRePlayButton:
				//�I�����Ă���{�^�����^�C�g���֖߂�{�^���ɂ���
				m_selectedSpriteType = enToTitleButton;
				break;
			case enToTitleButton:
				//�I�����Ă���{�^�������v���C�{�^���ɂ���
				m_selectedSpriteType = enRePlayButton;
				break;
			}
			//�Z���N�g���鉹���o��
			CSoundSource* selectSE = NewGO<CSoundSource>(0);
			selectSE->Init(L"Assets/sound/select.wav", false);
			selectSE->SetVolume(SE_VOLUME);
			selectSE->Play(false);
		}
		//���{�^����������Ă���
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			switch (m_selectedSpriteType)
			{
			case enRePlayButton:
				//�I�����Ă���{�^�����^�C�g���֖߂�{�^���ɂ���
				m_selectedSpriteType = enToTitleButton;
				break;
			case enToTitleButton:
				//�I�����Ă���{�^�������v���C�{�^���ɂ���
				m_selectedSpriteType = enRePlayButton;
				break;
			}
			//�Z���N�g���鉹���o��
			CSoundSource* selectSE = NewGO<CSoundSource>(0);
			selectSE->Init(L"Assets/sound/select.wav", false);
			selectSE->SetVolume(SE_VOLUME);
			selectSE->Play(false);
		}
	}
}

void AfterGameOverScene::Update()
{
	if (m_blackSprite == nullptr)
	{
		m_blackSprite = NewGO<SpriteRender>(0);
		m_blackSprite->Init("Assets/image/fade/black.dds", 1280, 720);
		m_blackSprite->SetColor(BLACK);
	}
	else
	{
		m_alpha += GameTime().GetFrameDeltaTime();
		if (m_alpha >= BACKGROUND_DARKNESS)
		{
			m_alpha = BACKGROUND_DARKNESS;
		}
		Vector4 color = BLACK;
		color.w = m_alpha;
		m_blackSprite->SetColor(color);
	}

	//�㉺�{�^���ŉ�ʏ�{�^����I������
	SelectButton();

	//���ݑI�����Ă���{�^���摜�����F������
	for (int i = 0; i < enSpriteNum; i++)
	{
		//�I�����Ă���ԍ��ƈ�v���Ă�����
		if (i == m_selectedSpriteType)
		{
			//���F�ɂ���
			m_sprite[i]->SetSpriteAlpha(1.0f);
		}
		else
		{
			//�ʏ�̐F�ɂ���
			m_sprite[i]->SetSpriteAlpha(0.0f);
		}
	}

	//A�{�^������������
	if (g_pad[0]->IsTrigger(enButtonA)
		&& m_fade == nullptr)
	{
		//�t�F�[�h�C������
		m_fade = NewGO<Fade>(0);
		//�{�^���𓮂����Ȃ��悤�ɂ���
		m_canSelectButton = false;
	}

	//�t�F�[�h�C�����I����đҋ@��ԂȂ��
	if (m_fade != nullptr && m_fade->GetFadePhase() == enWait)
	{
		//�������܂ł��Ă����Q�[������������
		Game* pastGame = FindGO<Game>("game");
		//�j������
		DeleteGO(pastGame);
		//�t�F�[�h�A�E�g����悤�ɓ`����
		m_fade->SetFadeOut();
		//���̃V�[���ɑJ�ڂ���
		m_shouldStartNextScene = true;
		//���̉摜�Q������
		DeleteGO(this);
	}
	if (m_shouldStartNextScene)
	{
		//�����ꂽ�{�^�������v���C�{�^���Ȃ��
		if (m_selectedSpriteType == enRePlayButton)
		{
			//�V�����Q�[�����쐬
			Game* newGame = NewGO<Game>(0, "game");
		}
		//�^�C�g���֖߂�{�^���Ȃ��
		else if (m_selectedSpriteType == enToTitleButton)
		{
			//�^�C�g�����쐬
			Title* newTitle = NewGO<Title>(0, "title");
		}
	}
}