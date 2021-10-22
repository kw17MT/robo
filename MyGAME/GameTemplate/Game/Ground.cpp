#include "stdafx.h"
#include "Ground.h"
#include "SkinModelRender.h"

Ground::~Ground()
{
	DeleteGO(m_skinModelRender);
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitGround("Assets/modelData/ground/ground.tkm", enModelUpAxisZ, { 0.0f,-2000.0f,0.0f }, false);
	m_skinModelRender->SetPosition({ 0.0f,-50000.0f,0.0f });
	m_skinModelRender->SetScale({ 1.0f,1.0f,1.0f });
	m_skinModelRender->SetIsGround();

	//m_physics.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	return true;
}