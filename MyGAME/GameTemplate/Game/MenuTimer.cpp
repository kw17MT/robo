#include "stdafx.h"
#include "MenuTimer.h"
#include "SkinModelRender.h"
#include "GameDirector.h"

MenuTimer::~MenuTimer()
{
	DeleteGO(m_skinModelRender);
}

bool MenuTimer::Start()
{
	m_lig.ambientLight.Set(0.0f, 100.0f, 0.0f);
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position, &m_lig);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_skinModelRender->SetRotation(m_rot);
	return true;
}

void MenuTimer::ResetTimerParam()
{
	timer = 1000;
	m_scale.x = 2.8f;
	m_lig.ambientLight.Set(0.0f, 100.0f, 0.0f);
	isTimeUp = false;
}


void MenuTimer::Update()
{
	if (GetGameDirector().GetGameScene() != enGamePlay) {
		return;
	}

	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetPosition(m_position);

	if (GetGameDirector().GetIsGameCountDown())
	{
		//処理しない。
		return;
	}

	m_scale.x -= 0.001f;
	//m_scale.x -= 0.01f;
	timer--;
	//timer -= 10;
	//m_lig.ambientLight.y -= 0.1f;

	//ゲージが1/3までは赤成分を足す = 黄色にする
	if (m_scale.x >= 0.9f) {
		m_lig.ambientLight.x += 0.001f;
		if (m_lig.ambientLight.x >= 2.0f) {
			m_lig.ambientLight.x = 2.0f;
		}
	}
	if (m_scale.x < 1.0f) {
		m_lig.ambientLight.y -= 0.4f;
		if (m_lig.ambientLight.y <= 0.0f) {
			m_lig.ambientLight.y = 0.0f;
		}
	}

	if (m_scale.x <= 0.0f) {
		//timer = 1000;
		//m_scale.x = 2.8f;
		//m_lig.ambientLight.Set(0.0f, 100.0f, 0.0f);
		isTimeUp = true;
	}
}