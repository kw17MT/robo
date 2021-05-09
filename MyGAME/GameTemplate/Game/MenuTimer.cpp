#include "stdafx.h"
#include "MenuTimer.h"
#include "SkinModelRender.h"

bool MenuTimer::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	//m_rot.SetRotationDegY(90.0f);
	m_skinModelRender->SetRotation(m_rot);
	return true;
}

void MenuTimer::ResetTimerParam()
{
	timer = 1000;
	m_scale.x = 2.8f;
	isTimeUp = false;
}

void MenuTimer::Update()
{
	m_scale.x -= 0.001f;
	timer--;
	if (m_scale.x <= 0.0f) {
		timer = 1000;
		m_scale.x = 2.8f;

		isTimeUp = true;
	}
	
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetPosition(m_position);
}