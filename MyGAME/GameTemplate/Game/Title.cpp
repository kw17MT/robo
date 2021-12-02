#include "stdafx.h"
#include "Title.h"
#include "SpriteRender.h"
#include "SkinModelRender.h"
#include "GameDirector.h"

#include "Game.h"
#include "LaunchScene.h"

Title::~Title()
{
	for (int i = 0; i < enSpriteNum; i++)
	{
		DeleteGO(m_sprite[i]);
	}

	DeleteGO(m_robo);
}

bool Title::Start()
{
	GameDirector::CreateInstance();

	for (int i = 0; i < enSpriteNum; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(0);
	}

	m_sprite[enTitle]->Init("Assets/Image/warning/AreaWarn1.dds", 250, 150, "PSWarningSign");
	m_sprite[enTitle]->SetPosition({ 600.0f, 300.0f,0.0f });
	m_sprite[enPlayButton]->Init("Assets/Image/warning/AreaWarn1.dds", 250, 150, "PSWarningSign");
	m_sprite[enPlayButton]->SetPosition({ -600.0f, 300.0f,0.0f });
	m_sprite[enHowToButton]->Init("Assets/Image/warning/AreaWarn1.dds", 250, 150, "PSWarningSign");
	m_sprite[enHowToButton]->SetPosition({ -600.0f, 200.0f,0.0f });
	m_sprite[enExitButton]->Init("Assets/Image/warning/AreaWarn1.dds", 250, 150, "PSWarningSign");
	m_sprite[enExitButton]->SetPosition({ -600.0f, 100.0f,0.0f });

	m_robo = NewGO<SkinModelRender>(0);
	m_robo->Init("Assets/modelData/test2.tkm", "Assets/modelData/test2.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_robo->SetScale({ 5.0f,5.0f,5.0f });

	g_camera3D->SetPosition({ 0.0f,100.0f,300.0f });
	g_camera3D->SetTarget(Vector3::Zero);

	return true;
}

void Title::Update()
{
	if (float dir = g_pad[0]->GetRStickXF())
	{
		if (dir > 0.0f)
		{
			angleX += 0.001f;
		}
		else if (dir < 0.0f)
		{
			angleX -= 0.001f;
		}
	}

	float dif = 0.0f - angleX;
	dif /= 20.0f;
	angleX += dif;
	Quaternion qRot;
	qRot.SetRotationY(angleX);
	g_camera3D->RotateOriginTarget(qRot);

	if (float dir = g_pad[0]->GetRStickYF())
	{
		if (dir > 0.0f)
		{
			angleY += 0.001f;
		}
		else if (dir < 0.0f)
		{
			angleY -= 0.001f;
		}
	}

	dif = 0.0f - angleY;
	dif /= 20.0f;
	angleY += dif;
	qRot.SetRotation(g_camera3D->GetRight(), angleY);
	Vector3 nextCameraForward = g_camera3D->GetForward();
	qRot.Apply(nextCameraForward);
	if (nextCameraForward.Dot(g_vec3AxisY) < 0.8f
		&& nextCameraForward.Dot(g_vec3AxisY) > -0.7f)
	{
		g_camera3D->RotateOriginTarget(qRot);
	}
	else
	{
		angleY = 0.0f;
	}

	if (g_pad[0]->IsPress(enButtonA))
	{
		DeleteGO(this);
		NewGO<LaunchScene>(0);
	}
}