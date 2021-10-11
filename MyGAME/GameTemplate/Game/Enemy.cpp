#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");
	m_skinModelRender = NewGO<SkinModelRender>(0, "enemy");
	m_skinModelRender->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_position = { 0.0f,400.0f,-5000.0f };

	m_displayDistance = NewGO<DisplayDistance>(10);
	m_displayDistance->SetEnemyPos(m_position);

	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
}