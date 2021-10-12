#include "stdafx.h"
#include "Reticle.h"

namespace
{
	const Vector3 SCALE_CHANGE_AMOUNT = { 0.1f,0.1f,0.1f };
}

bool Reticle::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_idle.dds", 48, 48);
	m_spriteRender[0]->SetPosition(m_homePosition);

	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_target.dds", 48, 48);
	m_spriteRender[1]->SetPosition(m_homePosition);

	return true;
}

void Reticle::Update()
{
	/*
	* if(targeting){
	//g_camera3D->CalcScreenPositionFromWorldPosition(m_lockOnPosition, enemyPos);
	Vector3 targetPos = Vector3::Zero;
	targetPos.xy = m_lockOnPosition;
	m_spriteRender.SetPosition(targetPos);
	}
	if(!targeting)
	m_sprtiteRender->SetPosition(m_homePosition);
	*/

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		/*if (m_isTarget == 0)
		{
			m_isTarget = 1;
		}
		else {
			m_isTarget = 0;
		}*/

		m_isTarget = !m_isTarget;
	}

	if (m_isTarget == 1)
	{
		g_camera3D->CalcScreenPositionFromWorldPosition(m_lockOnPosition, /*enemyPos*/{0.0f, 400.0f, -5000.0f});
		Vector3 targetPos = Vector3::Zero;
		targetPos.x = -m_lockOnPosition.x;
		targetPos.y = m_lockOnPosition.y;
		m_spriteRender[1]->SetPosition(targetPos);


		m_reticleScale[1] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x >= 1)
		{
			m_reticleScale[1] = Vector3::One;
		}

		m_reticleScale[0] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x <= 0)
		{
			m_reticleScale[0] = Vector3::Zero;
		}
		m_spriteRender[0]->SetScale(m_reticleScale[0]);
		m_spriteRender[1]->SetScale(m_reticleScale[1]);
	}
	else
	{
		m_reticleScale[1] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x <= 0)
		{
			m_reticleScale[1] = Vector3::Zero;
		}

		m_reticleScale[0] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x >= 1)
		{
			m_reticleScale[0] = Vector3::One;
		}
		m_spriteRender[0]->SetScale(m_reticleScale[0]);
		m_spriteRender[1]->SetScale(m_reticleScale[1]);
	}

	m_spriteRender[0]->SetPosition(m_homePosition);
}