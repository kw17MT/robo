#include "stdafx.h"
#include "ModelRender_new.h"

void ModelRender_new::Init(const char* filePath,EnModelUpAxis modelUpAxis)
{
	m_modelInitData.m_tkmFilePath = filePath;
	m_modelInitData.m_modelUpAxis = modelUpAxis;

}

void ModelRender_new::SetPosition(Vector3& pos)
{
	m_model.UpdateWorldMatrix(pos, m_rotation, m_scale);
}

void ModelRender_new::SetRotation(Quaternion& rot)
{
	m_model.UpdateWorldMatrix(m_position, rot, m_scale);
}

void ModelRender_new::SetScale(Vector3& scale)
{
	m_model.UpdateWorldMatrix(m_position, m_rotation, scale);
}