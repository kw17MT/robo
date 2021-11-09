#include "stdafx.h"
#include "RocketTargetStateIcon.h"
#include "SpriteRender.h"

extern Vector3 CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

RocketTargetStateIcon::~RocketTargetStateIcon()
{
	DeleteGO(m_spriteRender);
}

bool RocketTargetStateIcon::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticle_near2.dds", 96, 96);

	return true;
}

void RocketTargetStateIcon::Update()
{
	//�G�̃X�N���[�����W���v�Z����
	m_screenPos = CalcMethods::CalcScreenPos(m_screenPos, m_targetedEnemyPos);

	m_spriteRender->SetPosition(m_screenPos);
	m_spriteRender->SetScale(m_scale);
}