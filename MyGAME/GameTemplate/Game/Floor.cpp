#include "stdafx.h"
#include "Floor.h"
#include "SkinModelRender.h"

bool Floor::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_lig.ambientLight.Set(2.5f, 2.5f, 2.5f);
	if(isRed) {
		m_skinModelRender->Init("Assets/modelData/floor/floor_red1.tkm", nullptr, enModelUpAxisZ, m_position, &m_lig);
	}
	else {
		m_skinModelRender->Init("Assets/modelData/floor/floor_blue1.tkm", nullptr, enModelUpAxisZ, m_position, &m_lig);
	}
	m_skinModelRender->InitShader("Assets/shader/demodel.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	return true;
}

void Floor::Update()
{
	m_skinModelRender->SetPosition(m_position);
}