#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "AmmoGauge.h"
#include "SoundSource.h"
#include "GameDirector.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 10.0f,10.0f,10.0f };
	const int MAX_AMMO = 100;
	const float SE_VOLUME = 1.0f;
}

MachinGun::~MachinGun()
{
	//弾数ゲージを削除
	DeleteGO(m_ammoGauge);
	//モデルの削除
	DeleteGO(m_skinModelRender);
}

bool MachinGun::Start()
{
	//マシンガンのモデル生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Gun/gun3.tkm", nullptr, enModelUpAxisZ, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	m_ammoGauge = NewGO<AmmoGauge>(0);
	m_ammoGauge->SetAmmoType(enAmmoBullet);
	m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	m_ammoGauge->SetPosition({ -440.0f, -280.0f, 0.0f });

	return true;
}

void MachinGun::Update()
{
	if (GameDirector::GetInstance().GetGameScene() == enGameOver)
	{
		m_skinModelRender->SetPosition(m_position);
		return;
	}
	m_shootDelay -= GameTime().GetFrameDeltaTime();

	//to do ステートメントで管理する事
	if (g_pad[0]->IsPress(enButtonRB2) && m_remaining_bullet > 0 && m_shootDelay <= 0.0f)
	{
		Bullet* bullet = NewGO<Bullet>(0);
		bullet->SetTargetAndCurrentPos(m_targetPos, m_position);
		bullet->SetOwner(enPlayer);
		Quaternion rot;
		rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_targetPos - m_position);
		bullet->SetRotation(rot);
		//残り弾数を1つマイナス
		m_remaining_bullet--;
		m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
		m_shootDelay = 0.15f;

		CSoundSource* shootSE = NewGO<CSoundSource>(0);
		shootSE->Init(L"Assets/sound/machingun.wav", false);
		shootSE->SetVolume(SE_VOLUME);
		shootSE->Play(false);

		LightManager::GetInstance().GiveLightForMachinGun(m_position, m_playerPos - m_position);
	}
	else
	{
		LightManager::GetInstance().TurnOffSpotLight();
	}

	if(m_ammoGauge->GetIsFinishedReloaded())
	{
		m_remaining_bullet = MAX_AMMO;
	}

	m_armDirection.Normalize();
	m_qRot.SetRotation({ -1.0f,0.0f,0.0f }, m_armDirection);

	Quaternion rotBasedY;
	rotBasedY.SetRotation({ -1.0f,0.0f,0.0f }, g_camera3D->GetForward());
	m_qRot.Apply(rotBasedY);

	m_position += m_armDirection * 20.0f;
	//モデルの位置を更新する
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetRotation(m_qRot);
}