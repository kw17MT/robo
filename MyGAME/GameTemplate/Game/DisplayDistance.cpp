#include "stdafx.h"
#include "DisplayDistance.h"

bool DisplayDistance::Start()
{
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"0");
	return true;
}

void DisplayDistance::CalcDistance(Vector3 enemyPos, Vector3 playerPos)
{
	m_enemyPos = enemyPos;
	Vector3 enemyToPlayer = enemyPos - playerPos;
	m_distance = enemyToPlayer.Length();
}

void DisplayDistance::Update()
{
	std::wstring distanceStr = std::to_wstring(m_distance);
	m_fontRender->SetText(distanceStr.c_str());

	g_camera3D->CalcScreenPositionFromWorldPosition(m_position, m_enemyPos);
	m_fontRender->SetPosition(m_position);
	m_fontRender->SetScale(0.4f);
}