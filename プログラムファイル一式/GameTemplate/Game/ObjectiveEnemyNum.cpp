#include "stdafx.h"
#include "ObjectiveEnemyNum.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "GameDirector.h"

namespace
{
	const Vector2 OBJECTIVE_FONT_POS = { 500.0f, 275.0f };
	const Vector2 KILLED_ENEMY_FONT_POS = { 475.0f, 275.0f };
	const Vector3 SPRITE_POS = { -550.0f,300.0f, 0.0f };
}

ObjectiveEnemyNum::~ObjectiveEnemyNum()
{
	DeleteGO(m_font[enObjectiveNum]);
	DeleteGO(m_font[enKilledEnemyNum]);
	DeleteGO(m_sprite);
}

bool ObjectiveEnemyNum::Start()
{
	m_font[enObjectiveNum] = NewGO<FontRender>(0);
	m_font[enObjectiveNum]->SetText(L"");
	m_font[enObjectiveNum]->SetPosition(OBJECTIVE_FONT_POS);
	m_font[enObjectiveNum]->SetScale(m_scale);

	m_font[enKilledEnemyNum] = NewGO<FontRender>(0);
	m_font[enKilledEnemyNum]->SetText(L"");
	m_font[enKilledEnemyNum]->SetPosition(KILLED_ENEMY_FONT_POS);
	m_font[enKilledEnemyNum]->SetScale(m_scale);

	m_sprite = NewGO<SpriteRender>(1);
	m_sprite->Init("Assets/image/playerUI/objective.dds", 250, 100);
	m_sprite->SetPosition(SPRITE_POS);

	return true;
}

void ObjectiveEnemyNum::Update()
{
	if (m_killedEnemyNum == m_objectiveNum)
	{
		GameDirector::GetInstance().SetGameScene(enGameClear);
	}

	std::wstring num = std::to_wstring(m_killedEnemyNum);
	m_font[enKilledEnemyNum]->SetText(num.c_str());

	num = std::to_wstring(m_objectiveNum);
	std::wstring slash = L" / ";
	slash += num;
	m_font[enObjectiveNum]->SetText(slash.c_str());
}