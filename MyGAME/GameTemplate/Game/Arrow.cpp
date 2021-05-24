#include "stdafx.h"
#include "Arrow.h"
#include "SkinModelRender.h"

Arrow::Arrow()
{

}

Arrow::~Arrow()
{
	DeleteGO(m_skinModelRender);
}

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

void Arrow::Expansion()
{
	m_scaleMinFlag = false;
	m_scale.x += 0.1f;
	m_scale.y += 0.1f;
	m_scale.z += 0.1f;

	if (m_scale.x > 1.0f || m_scale.y > 1.0f || m_scale.z > 1.0f) {
		m_scale = Vector3::One;
		m_scaleMaxFlag = true;
	}
}

void Arrow::Shrink()
{
	m_scaleMaxFlag = false;
	m_scale.x -= 0.1f;
	m_scale.y -= 0.1f;
	m_scale.z -= 0.1f;

	if (m_scale.x < 0.0f || m_scale.y < 0.0f || m_scale.z < 0.0f) {
		m_scale = Vector3::Zero;
		m_scaleMinFlag = true;
	}
}

void Arrow::Update()
{
	Float();
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);
}