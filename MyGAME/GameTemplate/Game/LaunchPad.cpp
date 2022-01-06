#include "stdafx.h"
#include "LaunchPad.h"
#include "SkinModelRender.h"
#include "GameDirector.h"

namespace
{
	const Vector3 HUMAN_SCALE = { 0.3f,0.3f,0.3f };
	const Vector3 HUMAN_POS = { -125.0f,235.0f,5300.0f };
}

LaunchPad::~LaunchPad()
{
	for (int i = 0; i < enPartsNum; i++)
	{
		DeleteGO(m_docParts[i]);
	}

	DeleteGO(m_man[0]);
	DeleteGO(m_man[1]);
}

bool LaunchPad::Start()
{
	for (int i = 0; i < enPartsNum; i++)
	{
		m_docParts[i] = NewGO<SkinModelRender>(0);
		m_docParts[i]->SetPosition(m_position);
		m_docParts[i]->SetScale(m_scale);
	}
	m_docParts[0]->Init("Assets/modelData/doc/tunnel.tkm", nullptr, enModelUpAxisZ, true);
	m_docParts[1]->Init("Assets/modelData/doc/wireAndPipe.tkm", nullptr, enModelUpAxisZ, false);
	m_docParts[2]->Init("Assets/modelData/doc/way.tkm", nullptr, enModelUpAxisZ, false);
	m_docParts[3]->Init("Assets/modelData/doc/doc_base.tkm", nullptr, enModelUpAxisZ, false);

	m_anim[0].Load("Assets/modelData/Man/clap.tka");
	m_anim[0].SetLoopFlag(true);
	m_anim[1].Load("Assets/modelData/Man/good.tka");
	m_anim[1].SetLoopFlag(true);

	Quaternion rot;
	rot.SetRotationY(145.0f);

	for (int i = 0; i < 2; i++)
	{
		m_man[i] = NewGO<SkinModelRender>(0);

		Vector3 pos = HUMAN_POS;
		pos.z += i * 10.0f;
		m_man[i]->SetPosition(pos);
		m_man[i]->SetScale(HUMAN_SCALE);
		m_man[i]->SetRotation(rot);

		//m_man[i]->InitAnimation(m_anim, 2);
		//m_man[i]->PlayAnimation(i);
	}
	m_man[0]->Init("Assets/modelData/Man/clapMan.tkm", "Assets/modelData/Man/clapMan.tks", enModelUpAxisZ, false);
	m_man[1]->Init("Assets/modelData/Man/goodMan.tkm", "Assets/modelData/Man/goodMan.tks", enModelUpAxisZ, false);
	m_man[0]->InitAnimation(m_anim, 2);
	m_man[0]->PlayAnimation(0);
	m_man[1]->InitAnimation(m_anim, 2);
	m_man[1]->PlayAnimation(1);

	for (int i = 0; i < 2; i++)
	{
		m_robo[i] = NewGO<SkinModelRender>(0);
		m_robo[i]->Init("Assets/modelData/testBox/test4.tkm", "Assets/modelData/testBox/test4.tks", enModelUpAxisZ, true);
	}
	Quaternion roboRot;
	roboRot.SetRotationY(135.0f);
	m_robo[0]->SetPosition({ 0.0f, 295.0f, 5800.0f });
	m_robo[0]->SetRotation(roboRot);
	roboRot.SetRotationY(90.0f);
	m_robo[1]->SetPosition({ -200.0f, 295.0f, 5600.0f });
	m_robo[1]->SetRotation(roboRot);

	return true;
}

void LaunchPad::Update()
{
	
}