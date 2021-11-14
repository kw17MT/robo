#include "stdafx.h"
#include "Rader.h"
#include "SpriteRender.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);
extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

Rader::~Rader()
{
	for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_spriteRender[i]);
	}

}

bool Rader::Start()
{
	for (int i = 0; i < 10; i++)
	{
		m_spriteRender[i] = NewGO<SpriteRender>(0);
		m_spriteRender[i]->Init("Assets/Image/HPBar/HPBar.dds", 4, 4);
		m_spriteRender[i]->SetPivot({ -125.0f, 50.0f });
		m_spriteRender[i]->SetScale(m_scale);
		//m_spriteRender[i]->SetPosition(m_screenPos);
	}

	m_playerIcon = NewGO<SpriteRender>(0);
	m_playerIcon->Init("Assets/Image/HPBar/HPBar.dds", 4, 4);
	m_playerIcon->SetPivot({ -125.0f, 50.0f });
	m_playerIcon->SetScale({ 1.0f,1.0f,1.0f });
	m_playerIcon->SetPosition({ 0.0f, 0.0f, 0.0f });

	return true;
}

void Rader::SaveEnemyPos(int no, Vector3 enemyPos)
{
	m_enemyPos[no] = enemyPos;
}

void Rader::Update()
{
	Quaternion qRot;
	qRot.SetRotation(g_camera3D->GetForward(), { 0.0f,0.0f,1.0f });

	for (int i = 0; i < m_enemyNum - 1; i++)
	{
		Vector3 playerToEnemyVec = m_enemyPos[i] - m_playerPos;
		
		playerToEnemyVec.Normalize();
		playerToEnemyVec.y = 0.0f;
		qRot.Apply(playerToEnemyVec);
		playerToEnemyVec.x *= -1.0f;
		playerToEnemyVec.y = playerToEnemyVec.z;
		playerToEnemyVec.z = 0.0f;



		float distance = CalcMethods::CalcDistance(m_enemyPos[i], m_playerPos);
		distance /= 100.0f;

		if (distance >= 100.0f)distance = 100.0f;
		

		m_screenPos = playerToEnemyVec * distance;

		m_spriteRender[i]->SetPosition(m_screenPos);
		m_scale = Vector3::One;
		m_spriteRender[i]->SetScale(m_scale);
	}
}