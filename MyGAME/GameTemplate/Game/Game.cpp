#include "stdafx.h"
#include "Game.h"
#include "EnemyGenerator.h"
#include "Player.h"
#include "SkyCube.h"
#include "Ground.h"
#include "UI.h";
#include "Sun.h"
#include "Rader.h"
#include "SoundSource.h"
#include "EliminateTelop.h"
#include "ObjectiveEnemyNum.h"
#include "Title.h"
#include "ClearTelop.h"
#include "Fade.h"
#include "GameDirector.h"
#include "CaptureStateManager.h"
#include "AfterGameOverScene.h"
#include "EnemyRepopManager.h"

namespace
{
	const int OBJECTIVE_NUM = 30;
	const float BGM_DECREASE_VOLUME = 0.001f;
	const float FADE_START_VOLUME = 24.0f;
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
	//�ڕW���j���\���C���X�^���X�폜
	DeleteGO(m_objectiveEnemyNum);
	//BGM�C���X�^���X�폜
	DeleteGO(m_bgm);
	//�N���A�\���C���X�^���X�폜
	DeleteGO(m_clear);
	//�t���O�Ǘ��C���X�^���X�폜
	CaptureStateManager::DeleteInstance();
	EnemyRepopManager::DeleteInstance();
}

bool Game::Start()
{
	//�G�Đ����}�l�[�W���[�̍쐬
	EnemyRepopManager::CreateInstance();
	EnemyRepopManager::GetInstance().ResetParam();

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
	//�c��G����\������
	m_objectiveEnemyNum = NewGO<ObjectiveEnemyNum>(0, "objective");
	//�Q�[���N���A�ƂȂ錂�j���̐ݒ�
	m_objectiveEnemyNum->SetObjectiveNum(OBJECTIVE_NUM);
	//�G������쐬
	m_enemyGenerator = NewGO<EnemyGenerator>(0);
	//BGM�쐬
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/bgm1.wav", false);
	m_bgm->SetVolume(m_bgmVolume);
	m_bgm->Play(true);

	//�Q�[�����V�[���ɑJ�ڂ���
	GameDirector::GetInstance().SetGameScene(enInGame);
	//�X�e�[�g�}�l�[�W���[�̍쐬
	CaptureStateManager::CreateInstance();


	return true;
}

void Game::Update()
{
	//���[�_�[�Ƀv���C���[�̈ʒu��^����
	m_rader->SetPlayerPos(m_player->GetRoboPosition());
	//���[�_�[�ɐ��������G�̐���^����
	m_rader->SetEnemyNum(m_enemyGenerator->GetEnemyNum());
	//�G�̐������̈ʒu���W��ۑ��A�X�V
	for (int i = 0; i < m_enemyGenerator->GetEnemyNum(); i++)
	{
		//�G�̈ʒu���W��ݒ�
		Vector4 enemyInfo = m_enemyGenerator->GetEnemyPos(i);
		//�G�������Ă��邩�ǂ�����w�ɓ����
		enemyInfo.w = m_enemyGenerator->GetIsEnemyAlive(i);
		//��L�������[�_�[�ɓn��
		m_rader->SaveEnemyPosAndIsAlive(i, enemyInfo);
	}

	//�Q�[���N���A�V�[���̎�
	if (GameDirector::GetInstance().GetGameScene() == enGameClear)
	{
		//�N���A��̌o�ߎ��Ԃ𑪒�
		m_colapsedTimeAfterClear += GameTime().GetFrameDeltaTime();
		//���X��BGM�̑傫�������炷
		m_bgmVolume -= BGM_DECREASE_VOLUME;
		//0�����ɂȂ�Ȃ��悤�ɂ���
		if (m_bgmVolume < 0.0f)
		{
			m_bgmVolume = 0.0f;
		}
		//BGM���ʂ�ݒ�
		m_bgm->SetVolume(m_bgmVolume);

		//�N���A�T�E���h��炵�Ă��Ȃ�������
		if (!m_isSoundClear)
		{
			//�N���A�T�E���h��炷
			CSoundSource* clearSE = NewGO<CSoundSource>(0);
			clearSE->Init(L"Assets/sound/clear.wav", false);
			clearSE->SetVolume(1.0f);
			clearSE->Play(false);
			m_isSoundClear = true;
		}
		//�N���A�e���b�v���o���Ă��Ȃ�������
		if (m_clear == nullptr)
		{
			//�N���A�e���b�v�C���X�^���X�̐���
			m_clear = NewGO<ClearTelop>(0);
		}
		//�N���A��̌o�ߎ��Ԃ����ȏ�ɂȂ��
		if (m_colapsedTimeAfterClear >= FADE_START_VOLUME)
		{
			//�t�F�[�h���J�n����
			if (m_fade == nullptr)
			{
				m_fade = NewGO<Fade>(0);
			}
			//�t�F�[�h�̒i�K���ҋ@��ԂȂ��
			if (m_fade->GetFadePhase() == enWait)
			{
				//�t�F�[�h�A�E�g���閽�߂��o��
				m_fade->SetFadeOut();
				//�^�C�g����ʂ��쐬
				Title* title = NewGO<Title>(0);
				//�v���C�����Q�[���V�[�����폜
				DeleteGO(this);
			}
		}
	}

	//�Q�[���I�[�o�[�V�[���Ȃ��
	if (GameDirector::GetInstance().GetGameScene() == enGameOver)
	{
		//BGM���ʂ������Ă���
		m_bgmVolume -= BGM_DECREASE_VOLUME;
		//0�����ɂȂ�Ȃ��悤�ɂ���
		if (m_bgmVolume < 0.0f)
		{
			m_bgmVolume = 0.0f;
		}
		//BGM���ʂ�ݒ肷��
		m_bgm->SetVolume(m_bgmVolume);
	}
	//�Q�[���I�[�o�[���o���I����Ă�����
	if(GameDirector::GetInstance().GetGameScene() == enAfterGameOver)
	{
		//�J�ڗU���C���X�^���X���쐬����
		m_afterGOScene = NewGO<AfterGameOverScene>(0);
	}
}