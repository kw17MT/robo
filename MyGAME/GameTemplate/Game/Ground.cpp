#include "stdafx.h"
#include "Ground.h"
#include "SkinModelRender.h"

namespace
{
	const Vector3 POSITION = { 0.0f,-50000.0f,0.0f };
	const Vector3 SCALE = Vector3::One;
}

Ground::~Ground()
{
	DeleteGO(m_skinModelRender);
}

bool Ground::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitGround("Assets/modelData/ground/ground.tkm", enModelUpAxisZ, Vector3::Zero, false);
	m_skinModelRender->SetPosition(POSITION);
	m_skinModelRender->SetScale(SCALE);
	m_skinModelRender->SetIsGround();

	//m_physics.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	return true;
}