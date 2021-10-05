#include "stdafx.h"
#include "Ground.h"
#include "SkinModelRender.h"

bool Ground::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/ground/ground.tkm", nullptr, enModelUpAxisZ, { 0.0f,-2000.0f,0.0f }, false);
	m_skinModelRender->SetPosition({ 0.0f,-50000.0f,0.0f });
	m_skinModelRender->SetScale({ 1.0f,1.0f,1.0f });
	m_skinModelRender->SetIsGround();

	return true;
}
