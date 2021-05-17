#include "stdafx.h"
#include "Arrow.h"
#include "SkinModelRender.h"

bool Arrow::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void Arrow::Float()
{
	m_timer++;
	if (m_timer < 25) {
		m_position.y += 1.0f;
	}
	if (m_timer >= 25 && m_timer <= 50) {
		m_position.y -= 1.0f;
	}
	if(m_timer > 50) {
		m_timer = 0;
	}

}

void Arrow::Update()
{
	Float();
	m_skinModelRender->SetPosition(m_position);
}