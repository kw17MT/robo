#include "stdafx.h"
#include "DisplayDistance.h"

bool DisplayDistance::Start()
{
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"0");
	return true;
}

const int DisplayDistance::CalcDistance(Vector3 enemyPos, Vector3 playerPos) 
{
	m_enemyPos = enemyPos;
	Vector3 enemyToPlayer = enemyPos - playerPos;
	m_distance = enemyToPlayer.Length();

	return m_distance;
}

void DisplayDistance::Update()
{
	std::wstring distanceStr = std::to_wstring(m_distance);
	m_fontRender->SetText(distanceStr.c_str());

	g_camera3D->CalcScreenPositionFromWorldPosition(m_position, m_enemyPos);
	//敵の位置とカメラの前方向の内積によってアイコンを映すか決める
	//敵からカメラへのベクトル作成
	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//正規化
	enemyToCamera.Normalize();
	//敵の位置とカメラの前方向の内積
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//敵がカメラの前方向にあるならば映す
	if (dot < 0.0f)
	{
	}
	else
	{
		m_position = { -1000.0f, -1000.0f };
	}
	m_fontRender->SetPosition(m_position);
	m_fontRender->SetScale(0.4f);
}