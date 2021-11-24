#include "stdafx.h"
#include "UI.h"

namespace
{
	const int ICON_SIZE = 24;
	const int FRAME_SIZE_X = 800;
	const int FRAME_SIZE_Y = 640;
	const int PLAYER_FRAME_SIZE_X = 500;
	const int PLAYER_FRAME_SIZE_Y = 100;
	const int FIXED_UI_NUM = 4;
	const Vector3 AMMO_FRAME = { -480.0f, -270.0f, 0.0f };
	const Vector3 PLAYER_FRAME = { 0.0f, -300.0f,0.0f };
	const Vector3 BULLET_ICON = { -415.0f, -275.0f, 0.0f };
	const Vector3 MISSILE_ICON = { -415.0f, -245.0f, 0.0f };
}

UI::~UI()
{
	for (int i = 0; i < FIXED_UI_NUM; i++)
	{
		DeleteGO(m_sprite[i]);
	}
}

bool UI::Start()
{
	for (int i = 0; i < FIXED_UI_NUM; i++)
	{
		m_sprite[i] = NewGO<SpriteRender>(0);
	}

	//残弾数用フレーム
	m_sprite[0]->Init("Assets/Image/ammoUI/ammoFrame.dds", FRAME_SIZE_X, FRAME_SIZE_Y);
	m_sprite[0]->SetPosition(AMMO_FRAME);
	//プレイヤーのHPバーの後ろ
	m_sprite[1]->Init("Assets/Image/PlayerUI/frame.dds", PLAYER_FRAME_SIZE_X, PLAYER_FRAME_SIZE_Y);
	m_sprite[1]->SetPosition(PLAYER_FRAME);
	//弾丸のアイコン
	m_sprite[2]->Init("Assets/Image/ammoUI/bulletIcon.dds", ICON_SIZE, ICON_SIZE);
	m_sprite[2]->SetPosition(BULLET_ICON);
	//ミサイルのアイコン
	m_sprite[3]->Init("Assets/Image/ammoUI/missileIcon.dds", ICON_SIZE, ICON_SIZE);
	m_sprite[3]->SetPosition(MISSILE_ICON);


	return true;
}

void UI::Update()
{
}