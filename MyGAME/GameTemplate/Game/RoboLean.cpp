#include "stdafx.h"
#include "RoboLean.h"
#include "SkinModelRender.h"

void RoboLean::Lean(SkinModelRender* skinModel)
{
	if (g_pad[0]->GetLStickXF() < 0.0f)
	{
		Quaternion qRot;
		if (m_leanDegree < 15.0f) {
			m_leanDegree += 1.0f;

			qRot.SetRotationDeg(g_camera3D->GetForward(), m_leanDegree);
			skinModel->SetRotation(qRot);
		}
		if (m_leanDegree >= 15.0f && m_timer < 120)
		{
			m_timer++;
		}
		if (m_leanDegree >= 15.0f && m_timer >= 120)
		{
			m_leanDegree += 2.0f;
			if (m_leanDegree >= 30.0f) {
				return;
			}
			qRot.SetRotationDeg(g_camera3D->GetForward(), m_leanDegree);
			skinModel->SetRotation(qRot);
		}
	}
}