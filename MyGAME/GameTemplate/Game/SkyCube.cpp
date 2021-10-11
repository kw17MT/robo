#include "stdafx.h"
#include "SkyCube.h"
#include "SkinModelRender.h"

bool SkyCube::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitSkyCube("Assets/modelData/preset/sky.tkm", enModelUpAxisZ);
	m_skinModelRender->SetPosition({ 0.0f,0.0f,0.0f });
	m_skinModelRender->SetScale({ 1000.0f,1000.0f,1000.0f });
	m_skinModelRender->SetIsSky();

	return true;
}

void SkyCube::Update()
{
	m_skinModelRender->SetPosition({ 0.0f,0.0f,0.0f });
	m_skinModelRender->SetScale({ 100000.0f,100000.0f,100000.0f });
}