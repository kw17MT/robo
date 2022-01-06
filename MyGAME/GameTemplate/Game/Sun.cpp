#include "stdafx.h"
#include "Sun.h"
#include "SkinModelRender.h"

Sun::~Sun()
{
	DeleteGO(m_skinModelRender);
}

bool Sun::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/sun/sun.tkm", nullptr, enModelUpAxisZ, false);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetIsSun();

	return true;
}

void Sun::Update()
{

}