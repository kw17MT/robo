#include "stdafx.h"
#include "Reticle.h"

bool Reticle::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/square.dds", 16, 16);
	m_spriteRender->SetPosition(m_homePosition);

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

	if (g_pad[0]->IsPress(enButtonB))
	{
		g_camera3D->CalcScreenPositionFromWorldPosition(m_lockOnPosition, { 0.0f, 400.0f, -5000.0f });
		Vector3 targetPos = Vector3::Zero;
		targetPos.x = -m_lockOnPosition.x;
		targetPos.y = m_lockOnPosition.y;
		m_spriteRender->SetPosition(targetPos);
	}
	else {
		m_spriteRender->SetPosition(m_homePosition);
	}
}