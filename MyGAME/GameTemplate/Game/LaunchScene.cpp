#include "stdafx.h"
#include "LaunchScene.h"
#include "SkinModelRender.h"
#include "GameDirector.h"
#include "Game.h"
#include "LaunchPad.h"
#include "SkyCube.h"
#include "PlayerEffect.h"
#include "Sun.h"

LaunchScene::~LaunchScene()
{
	DeleteGO(m_launchPad);
	DeleteGO(m_sky);
	DeleteGO(m_robo);
	DeleteGO(m_sun);
	DeleteGO(m_effect);

	for (int i = 0; i < 8; i++)
	{
		DeleteGO(m_lamp[i]);
	}
}

bool LaunchScene::Start()
{
	m_launchPad = NewGO<LaunchPad>(0);
	m_sky = NewGO<SkyCube>(0);
	m_effect = NewGO<PlayerEffect>(0);
	m_sun = NewGO<Sun>(0);

	m_robo = NewGO<SkinModelRender>(0);
	m_robo->Init("Assets/modelData/robo/robo3.tkm", "Assets/modelData/robo/robo3.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_robo->SetPosition(m_roboPos);
	Quaternion qRot;
	qRot.SetRotation({ 0.0f,0.0f,1.0f }, { 0.0f,0.0f,-1.0f });
	m_robo->SetRotation(qRot);

	for (int i = 0; i < 8; i++)
	{
		m_lamp[i] = NewGO<SkinModelRender>(0);
		m_lamp[i]->Init("Assets/modelData/doc/lamp.tkm", nullptr, enModelUpAxisZ, g_vec3Zero, false);
		Vector3 pos = m_lampPos;
		pos.y -= 65.0f;
		if (i >= 0 && i < 4)
		{
			pos.z -= 400.0f * (i + 1);
			pos.x -= 60.0f;
			m_lamp[i]->SetPosition(pos);
		}
		else
		{
			pos.z -= 400.0f * (i - 3);
			pos.x += 60.0f;
			m_lamp[i]->SetPosition(pos);
		}
	}

	g_camera3D->SetPosition({ 0.0f,400.0f, 5200.0f });
	g_camera3D->SetTarget(m_roboPos);

	return true;
}

void LaunchScene::Update()
{
	m_delay += GameTime().GetFrameDeltaTime();
	if (m_delay >= 3.0f
		&& m_delay <= 5.0f)
	{
		Quaternion qRot;
		qRot.SetRotationY(0.04f);
		g_camera3D->RotateOriginTarget(qRot);
	}
	else if (m_delay > 7.0f)
	{
		m_roboPos.z -= 10.0f;

		if (m_delay > 13.0f)
		{
			m_roboPos.y += 2.0f;
			m_effect->SetIsDash(true);
			m_effect->CalcRotation({ 0.0,0.0,-100.0f });
		}
	}

	m_effect->SetBothBoosterPos(
		m_robo->GetBonePosition(L"BoosterR"),
		m_robo->GetBonePosition(L"BoosterL")
	);

	m_effect->SetBothShoulderPos(
		m_robo->GetBonePosition(L"Bone044"),
		m_robo->GetBonePosition(L"Bone024")
	);

	m_robo->SetPosition(m_roboPos);
	g_camera3D->SetTarget(m_roboPos);

	if (m_roboPos.z <= 1000.0f)
	{
		GameDirector::GetInstance().SetGameScene(enInGame);
		DeleteGO(this);

		LightManager::GetInstance().TurnOffSpotLight();
		//フェードから遷移すること
		Game* a = NewGO<Game>(0);

		//発射処理がすべて終わったためゲームを進行させる。
		//GameDirector::GetInstance().SetGameScene(enInGame);
	}
}