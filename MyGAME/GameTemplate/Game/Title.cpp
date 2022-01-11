#include "stdafx.h"
#include "Title.h"
#include "SpriteRender.h"
#include "SkinModelRender.h"
#include "GameDirector.h"
#include "SoundSource.h"

#include "Game.h"
#include "LaunchScene.h"

namespace
{
	const int BUTTON_SIZE_X = 150;
	const int BUTTON_SIZE_Y = 80;
	const float BGM_VOLUME = 0.5f;
	const float SE_VOLUME = 0.5f;
}

Title::~Title()
{
	for (int i = 0; i < enSpriteNum; i++)
	{
		DeleteGO(m_sprite[i]);
	}

	DeleteGO(m_robo);
	DeleteGO(m_bunker);
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	GameDirector::CreateInstance();

	for (int i = 0; i < enSpriteNum; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(0);
	}

	m_sprite[enTitle]->Init("Assets/Image/Title/titleRogo.dds", 450, 250);
	m_sprite[enTitle]->SetPosition({ 400.0f, 220.0f,0.0f });
	m_sprite[enTitle]->SetSpriteAlpha(0.0f);
	m_sprite[enPlayButton]->Init("Assets/Image/Title/Play.dds", BUTTON_SIZE_X, BUTTON_SIZE_Y, "PSWarningSign");
	m_sprite[enPlayButton]->SetPosition({ -460.0f, -100.0f,0.0f });
	m_sprite[enPlayButton]->SetSpriteAlpha(0.0f);
	m_sprite[enHowToButton]->Init("Assets/Image/Title/HowTo.dds", 350, BUTTON_SIZE_Y, "PSWarningSign");
	m_sprite[enHowToButton]->SetPosition({ -450.0f, -200.0f,0.0f });
	m_sprite[enHowToButton]->SetSpriteAlpha(0.0f);
	m_sprite[enExitButton]->Init("Assets/Image/Title/Exit.dds", BUTTON_SIZE_X, BUTTON_SIZE_Y, "PSWarningSign");
	m_sprite[enExitButton]->SetPosition({ -460.0f, -300.0f,0.0f });
	m_sprite[enExitButton]->SetSpriteAlpha(0.0f);

	m_robo = NewGO<SkinModelRender>(0);
	m_robo->Init("Assets/modelData/testBox/test4.tkm", "Assets/modelData/testBox/test4.tks", enModelUpAxisZ, true);
	m_robo->SetPosition({ -50.0f, 100.0f,0.0f });
	m_robo->SetScale({ 5.0f,5.0f,5.0f });

	m_bunker = NewGO<SkinModelRender>(0);
	m_bunker->Init("Assets/modelData/Bunker/testO.tkm", nullptr, enModelUpAxisZ, false);
	m_bunker->SetScale({ 10.0f,10.0f,10.0f });

	g_camera3D->SetPosition({ 0.0f,300.0f,500.0f });
	g_camera3D->SetTarget(Vector3::Zero);	
	Quaternion qRot;
	qRot.SetRotationY(-0.5f);
	g_camera3D->RotateOriginTarget(qRot);

	//m_cameraShake.SetParam(10.0f, 0.1f, true);

	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/title.wav", false);
	m_bgm->SetVolume(BGM_VOLUME);
	m_bgm->Play(true);
	


	return true;
}

void Title::Update()
{
	if (float dir = g_pad[0]->GetRStickXF())
	{
		angleX += -0.0007f * dir;
	}
	//m_cameraShake.SetHomePosition({ 0.0f,300.0f,500.0f });
	//g_camera3D->SetPosition(g_camera3D->GetPosition() + m_cameraShake.Shake());

	float dif = 0.0f - angleX;
	dif /= 20.0f;
	angleX += dif;
	Quaternion qRot;
	qRot.SetRotationY(angleX);
	g_camera3D->RotateOriginTarget(qRot);

	if (float dir = g_pad[0]->GetRStickYF())
	{
		angleY += 0.0005f * dir;

	}

	dif = 0.0f - angleY;
	dif /= 20.0f;
	angleY += dif;
	qRot.SetRotation(g_camera3D->GetRight(), angleY);
	Vector3 nextCameraForward = g_camera3D->GetForward();
	qRot.Apply(nextCameraForward);
	if (nextCameraForward.Dot(g_vec3AxisY) < 0.2f
		&& nextCameraForward.Dot(g_vec3AxisY) > -0.7f)
	{
		g_camera3D->RotateOriginTarget(qRot);
	}
	else
	{
		angleY = 0.0f;
	}

	if (g_pad[0]->IsPress(enButtonA)
		&& m_enSelectedSpriteType == enPlayButton)
	{
		CSoundSource* selectSE = NewGO<CSoundSource>(0);
		selectSE->Init(L"Assets/sound/decide2.wav", false);
		selectSE->SetVolume(2.0f);
		selectSE->Play(false);

		CSoundSource* selectSE2 = NewGO<CSoundSource>(0);
		selectSE2->Init(L"Assets/sound/decide.wav", false);
		selectSE2->SetVolume(SE_VOLUME / 2.0f);
		selectSE2->Play(false);

		DeleteGO(this);
		NewGO<LaunchScene>(0);
	}

	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		switch (m_enSelectedSpriteType)
		{
		case enPlayButton:
			m_enSelectedSpriteType = enExitButton;
			break;
		case enHowToButton:
			m_enSelectedSpriteType = enPlayButton;
			break;
		case enExitButton:
			m_enSelectedSpriteType = enHowToButton;
			break;
		}
		CSoundSource* selectSE = NewGO<CSoundSource>(0);
		selectSE->Init(L"Assets/sound/select.wav", false);
		selectSE->SetVolume(SE_VOLUME);
		selectSE->Play(false);
	}
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		switch (m_enSelectedSpriteType)
		{
		case enPlayButton:
			m_enSelectedSpriteType = enHowToButton;
			break;
		case enHowToButton:
			m_enSelectedSpriteType = enExitButton;
			break;
		case enExitButton:
			m_enSelectedSpriteType = enPlayButton;
			break;
		}
		CSoundSource* selectSE = NewGO<CSoundSource>(0);
		selectSE->Init(L"Assets/sound/select.wav", false);
		selectSE->SetVolume(SE_VOLUME);
		selectSE->Play(false);
	}

	for (int i = 1; i < 4; i++)
	{
		m_sprite[i]->SetSpriteAlpha(0.0f);
	}
	m_sprite[m_enSelectedSpriteType]->SetSpriteAlpha(1.0f);
	
}