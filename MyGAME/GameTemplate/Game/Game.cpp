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
#include "EliminateTelop.h"
#include "ObjectiveEnemyNum.h"
#include "Title.h"

#include "GameDirector.h"
#include "CaptureStateManager.h"

namespace
{

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
	DeleteGO(m_objectiveEnemyNum);
	DeleteGO(m_bgm);

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
	//�S�ł�����̃e���b�v
	m_eliminateTelop = NewGO<EliminateTelop>(0);
	//�G������쐬
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	//�c��G����\������
	m_objectiveEnemyNum = NewGO<ObjectiveEnemyNum>(0, "objective");
	m_objectiveEnemyNum->SetObjectiveNum(10);
	//BGM�쐬
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/bgm1.wav", false);
	m_bgm->SetVolume(m_bgmVolume);
	m_bgm->Play(true);

	GameDirector::GetInstance().SetGameScene(enInGame);
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

	if (GameDirector::GetInstance().GetGameScene() == enGameClear)
	{
		m_colapsedTimeAfterClear += GameTime().GetFrameDeltaTime();

		m_bgmVolume -= 0.01f;
		if (m_bgmVolume < 0.0f)
		{
			m_bgmVolume = 0.0f;
		}
		m_bgm->SetVolume(m_bgmVolume);

		if (!m_isSoundClear)
		{
			CSoundSource* clearSE = NewGO<CSoundSource>(0);
			clearSE->Init(L"Assets/sound/clear.wav", false);
			clearSE->SetVolume(1.0f);
			clearSE->Play(false);
			m_isSoundClear = true;
		}

		if (m_colapsedTimeAfterClear >= 24.0f)
		{
			Title* title = NewGO<Title>(0);
			DeleteGO(this);
		}
	}

	if (g_pad[0]->IsPress(enButtonSelect))
	{
		DeleteGO(this);
	}
}