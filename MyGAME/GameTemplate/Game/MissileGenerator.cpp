#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"
#include "AmmoGauge.h"
#include "Enemy.h"
#include "SoundSource.h"

namespace
{
	const float SE_VOLUME = 1.0f;
}

namespace
{
	const int MAX_AMMO = 20;				//�ő吔
	const Vector3 GAUGE_POSITION = { -440.0f, -250.0f, 0.0f };
}

MissileGenerator::~MissileGenerator()
{
	//���o�Ă���~�T�C�������ׂď���
	for (auto i : m_missiles)
	{
		DeleteGO(m_missiles.back());
		m_missiles.pop_back();
	}
	m_missiles.clear();
	//�e���Q�[�W���폜
	DeleteGO(m_ammoGauge);
}

void MissileGenerator::SaveTargetedEnemy(Enemy* enemy)
{
	//�����ƃ~�T�C���Ń��b�N�I���ł���Ȃ�
	if (m_canTargetMore) {
		//���b�N�I�����Ă����G�̐��𑝂₷
		m_lockOnTargetNum++;
		//��������ɒB���A����ȏネ�b�N�I���ł��Ȃ����
		if (m_lockOnTargetNum >= MAX_AMMO)
		{
			//�ł��Ȃ����Ƃ�ۑ�����
			m_canTargetMore = false;
		}
		//�ۑ��p�z��ɃC���X�^���X��ۑ�
		m_enemy.push_back(enemy);
	}
}

bool MissileGenerator::Start()
{
	//�c�e���Q�[�W���쐬
	m_ammoGauge = NewGO<AmmoGauge>(0);
	//�e�̃^�C�v���~�T�C���ɐݒ�
	m_ammoGauge->SetAmmoType(enAmmoMissile);
	//���ˉ\����������
	m_ammoGauge->SetRemainingAmmo(m_remaining_missile);
	//�����������������
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	//�ʒu�Œ�
	m_ammoGauge->SetPosition(GAUGE_POSITION);

	//todo �v�ύX���������@�␔�l
	//�~�T�C���C���X�^���X�i�[�z��̃��������m��
	m_missiles.reserve(100);

	return true;
}

void MissileGenerator::Update()
{
	//LightManager::GetInstance().TurnOffSpotLight();

	//�����ł̓A�C�R���������Ȃ��悤�ɐݒ�
	m_deleteMissileIcon = false;

	//�~�T�C���̓����[�h����Ă��邩
	if (m_ammoGauge->GetIsReloaded()) {
		//LB2�{�^���������Ă���Ȃ��
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//���˂̏������Ă����
			m_isPrepareLaunch = true;
		}
		//���˂̏������ł��Ă��āA�{�^���𗣂�����
		if (m_isPrepareLaunch && !g_pad[0]->IsPress(enButtonLB2))
		{
			//���ɔ��˂����~�T�C���̐����v�Z
			int alreadyLaunchedNum = MAX_AMMO - m_remaining_missile;
			//���܂Ń��b�N�I�����Ă��������炷�łɔ��ˍς݂̃~�T�C���̐������Z��
			//���̃t���[���Ŕ��˂��ׂ��~�T�C�������v�Z
			int LaunchNumInThisFrame = m_lockOnTargetNum - alreadyLaunchedNum;
			//���ː������P�b�g������
			for (int i = 0; i < LaunchNumInThisFrame; i++)
			{
				//�~�T�C������
				m_missiles.push_back(NewGO<Missile>(0));
				//�ǐՂ���G��������
				m_missiles.back()->SetEnemy(m_enemy.back());
				//���ˌ���ݒ�
				m_missiles.back()->SetLaunchedPos(m_launchPos);
				//�g�����G�̃C���X�^���X�͂���Ȃ��̂ŏ��j��
				m_enemy.pop_back();
			}
			//�G�̏�����x�S���Y���
			m_enemy.clear();

			//�c��e�����獡�̃t���[���Ŕ��˂����~�T�C���̐������Z
			m_remaining_missile -= LaunchNumInThisFrame;
			//�X�V�����c��e�����Q�[�W�ɋ�����
			m_ammoGauge->SetRemainingAmmo(m_remaining_missile);

			//���ˏ����̉���
			m_isPrepareLaunch = false;
			//���b�N�I���Ŏg�p���Ă����A�C�R��������
			m_deleteMissileIcon = true;

			//�������b�N�I�����Ă��Ȃ���Ԃɂ���
			CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);

			//�v����
			/*if (LaunchNumInThisFrame)
			{
				LightManager::GetInstance().GiveLightForMissile(m_launchPos + g_camera3D->GetForward() * 1000.0f, g_camera3D->GetForward() * -1.0f);
			}*/

			//1�ȏ�~�T�C���𔭎˂����特��1��o��
			if (LaunchNumInThisFrame > 0)
			{
				CSoundSource* launchSE = NewGO<CSoundSource>(0);
				launchSE->Init(L"Assets/sound/launch.wav", false);
				launchSE->SetVolume(SE_VOLUME);
				launchSE->Play(false);
			}
		}
	}
	//�c�e�����Ȃ��Ȃ�����
	if (m_remaining_missile == 0)
	{
		//�������b�N�I���ł��Ȃ�
		m_canTargetMore = false;
		//�O�a��Ԃɂ���
		CaptureStateManager::GetInstance().SetMissileTargetState(enFull);
	}
	//�Q�[�W�Ń����[�h���I�������
	if (m_ammoGauge->GetIsFinishedReloaded())
	{
		//�c�e�����ő�l�ŏ�����
		m_remaining_missile = MAX_AMMO;
		//���܂܂Ń��b�N�I�����Ă����������Z�b�g
		m_lockOnTargetNum = 0;
		//����Ƀ��b�N�I���ł���
		m_canTargetMore = true;
		//�������b�N�I�����Ă��Ȃ���Ԃɕς��Ă��
		CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
	}
}