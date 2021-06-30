#include "stdafx.h"
#include "Conveyor.h"
#include "SkinModelRender.h"

bool Conveyor::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->InitForRecieveShadow("Assets/modelData/Conveyor.tkm", nullptr, enModelUpAxisZ, m_position, nullptr);

	return true;
}

void Conveyor::Update()
{
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);
}