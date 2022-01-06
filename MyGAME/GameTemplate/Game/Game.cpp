#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"
#include "Rader.h"
#include "LaunchPad.h"
#include "SoundSource.h"

#include "GameDirector.h"
#include "CaptureStateManager.h"

namespace f
{
	const float BGM_VOLUME = 0.5f;
}

Game::~Game()
{
	//�v���C���[�C���X�^���X�폜
	DeleteGO(m_player); m_player = nullptr;
	//�G������C���X�^���X�폜
	DeleteGO(m_enemyGenerator); m_enemyGenerator = nullptr;
	//�X�J�C�L���[�u�}�b�v�C���X�^���X�폜
	DeleteGO(m_sky); m_sky = nullptr;
	//�n�ʃC���X�^���X�폜
	DeleteGO(m_ground); m_ground = nullptr;
	//UI�C���X�^���X�폜
	DeleteGO(m_ui); m_ui = nullptr;
	//���z�C���X�^���X�폜
	DeleteGO(m_sun); m_sun = nullptr;
	//���[�_�[�C���X�^���X�폜
	DeleteGO(m_rader); m_rader = nullptr;

	//�t���O�Ǘ��C���X�^���X�폜
	CaptureStateManager::DeleteInstance();
}

bool Game::Start()
{
	//�v���C���[����
	m_player = NewGO<Player>(0,"player");
	//�X�J�C�L���[�u�}�b�v�쐬
	m_sky = NewGO<SkyCube>(0);
	//�n�ʍ쐬
	m_ground = NewGO<Ground>(0);
	//���z�̍쐬
	m_sun = NewGO<Sun>(0);
	//���[�_�[�̍쐬
	m_rader = NewGO<Rader>(0, "rader");
	//UI�̍쐬
	m_ui = NewGO<UI>(0);
	//�G������쐬
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	//BGM�쐬
	/*m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/bgm1.wav", false);
	m_bgm->SetVolume(f::BGM_VOLUME);
	m_bgm->Play(true);*/

	//�X�e�[�g�}�l�[�W���[�̍쐬
	CaptureStateManager::CreateInstance();

	return true;
}

void Game::Update()
{
	//���[�_�[�Ƀv���C���[�̈ʒu��^����
	m_rader->SetPlayerPos(m_player->GetPosition());
	//���[�_�[�ɐ��������G�̐�
	m_rader->SetEnemyNum(m_enemyGenerator->GetEnemyNum());
	//�G�̐������̈ʒu���W��ۑ��A�X�V
	for (int i = 0; i < m_enemyGenerator->GetEnemyNum(); i++)
	{
		m_rader->SaveEnemyPos(i, m_enemyGenerator->GetEnemyPos(i));
	}

	if (g_pad[0]->IsPress(enButtonSelect))
	{
		DeleteGO(this);
	}
}