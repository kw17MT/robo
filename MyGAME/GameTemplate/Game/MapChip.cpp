#include "stdafx.h"
#include "MapChip.h"

void MapChip::Init(const char* path01, Vector3 pos, Vector3 scale, Quaternion rot)
{
	m_modeldata.m_tkmFilePath = path01;
	m_modeldata.m_fxFilePath = "Assets/shader/model.fx";
	m_modeldata.m_expandConstantBuffer = &m_lig;
	m_modeldata.m_expandConstantBufferSize = sizeof(m_lig);
	m_model.Init(m_modeldata);
	m_model.UpdateWorldMatrix(pos, rot, scale);
}


void MapChip::Draw()
{
	m_model.Draw(g_graphicsEngine->GetRenderContext());
}

void MapChip::Draw(RenderContext& rc)
{
	m_model.Draw(rc);
}