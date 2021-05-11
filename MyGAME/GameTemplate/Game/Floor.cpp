#include "stdafx.h"
#include "Floor.h"
#include "SkinModelRender.h"

bool Floor::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//m_skinModelRender->Init("Assets/modelData/floor/L.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	return true;
}

void Floor::Update()
{
	m_skinModelRender->SetPosition(m_position);
}