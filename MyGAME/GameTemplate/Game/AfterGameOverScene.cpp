#include "stdafx.h"
#include "AfterGameOverScene.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

AfterGameOverScene::~AfterGameOverScene()
{
	//�I�����̉摜���폜����
	for (int i = 0; i < enSpriteNum; i++)
	{
		DeleteGO(m_sprite[i]);
	}
}

bool AfterGameOverScene::Start()
{
	//�I�����̉摜������
	for (int i = 0; i < enSpriteNum; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(0);
	}
	//Replay�摜�ŏ�����
	m_sprite[enRePlayButton]->Init("Assets/image/Title/Play.dds", 100, 100, "PSWarningSign");
	//ToTitle�摜�ŏ�����
	m_sprite[enToTitleButton]->Init("Assets/image/Title/Exit.dds", 100, 100, "PSWarningSign");
	//�摜�̈ʒu��������
	m_sprite[enRePlayButton]->SetPosition(m_position);
	m_sprite[enToTitleButton]->SetPosition({ 0.0f,100.0f,0.0f });

	return true;
}

void AfterGameOverScene::Update()
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
		selectSE->SetVolume(1.0);
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
		selectSE->SetVolume(1.0f);
		selectSE->Play(false);
	}

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
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�t�F�[�h�C������
		m_fade = NewGO<Fade>(0);
		//�t�F�[�h�C�����I����đҋ@��ԂȂ��
		if (m_fade->GetFadePhase() == enWait)
		{
			//�������܂ł��Ă����Q�[������������
			Game* pastGame = FindGO<Game>("game");
			//�j������
			DeleteGO(pastGame);
			//�t�F�[�h�A�E�g����悤�ɓ`����
			m_fade->SetFadeOut();
		}
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
		//���̉摜�Q������
		DeleteGO(this);
	}
}