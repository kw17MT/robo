#include "stdafx.h"
#include "LaunchPad.h"
#include "SkinModelRender.h"
#include "GameDirector.h"

LaunchPad::~LaunchPad()
{
	for (int i = 0; i < enPartsNum; i++)
	{
		DeleteGO(m_model[i]);
	}
}

bool LaunchPad::Start()
{
	for (int i = 0; i < enPartsNum; i++)
	{
		m_model[i] = NewGO<SkinModelRender>(0);
		m_model[i]->SetPosition(m_position);
		m_model[i]->SetScale(m_scale);
	}

	m_model[0]->Init("Assets/modelData/doc/tunnel.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_model[1]->Init("Assets/modelData/doc/wireAndPipe.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);
	m_model[2]->Init("Assets/modelData/doc/way.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, false);

	return true;
}

void LaunchPad::Update()
{
	
}