#include "stdafx.h"
#include "MissileTargetIcon.h"
#include "SpriteRender.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

MissileTargetIcon::~MissileTargetIcon()
{
	DeleteGO(m_spriteRender);
}

bool MissileTargetIcon::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticle_missile.dds", 96, 96);

	return true;
}

void MissileTargetIcon::Update()
{
	//敵のスクリーン座標を計算する
	CalcMethods::CalcScreenPos(m_screenPos, m_targetedEnemyPos);

	if (m_isFirstExpand)
	{
		m_scale = { 2.0f, 2.0f, 1.0f };
	}
	m_scale -= {0.1f, 0.1f, 0.0f};
	if (m_scale.x <= 1.0f)
	{
		m_scale = { 1.0f,1.0f,1.0f };
	}
	m_spriteRender->SetPosition(m_screenPos);
	m_spriteRender->SetScale(m_scale);
}