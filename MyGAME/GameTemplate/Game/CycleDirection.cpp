#include "stdafx.h"
#include "CycleDirection.h"
#include "SpriteRender.h"
#include "GameDirector.h"
#include "system/system.h"

namespace{
	const Vector3 SPRITE_RIGHT = { -220.0f, 80.0f,0.0f };
	const Vector3 SPRITE_LEFT = { 220.0f, 80.0f,0.0f };
	const Vector3 SPRITE_UP = { 0.0f, 250.0f,0.0f };
	const Vector4 SPRITE_INVISIBLE = { 1.0f,1.0f,1.0f,0.0f };
	
	const Vector3 FIXEDSPRITE_SCALE = { 0.25f, 0.25f, 1.0f };
	const Vector2 SPRITE_PIVOT = { 0.5f,0.5f };

	const int WIDTH = 512;
	const int HEIGHT = 512;
	
	const float SPRITE_ANGLE_CHANGE_RATE = 2.0f;
}

CycleDirection::~CycleDirection()
{
	DeleteGO(m_sprite);
}

bool CycleDirection::Start()
{
	m_sprite = NewGO<SpriteRender>(0);

	//スプライトを決定
	switch (m_direction) {
	case Forward:
		m_sprite->Init("Assets/Image/forward_dir.dds", WIDTH, HEIGHT);
		break;
	case Reverse:
		m_sprite->Init("Assets/Image/reverse_dir.dds", WIDTH, HEIGHT);
		break;
	case FixedForward:
		m_sprite->Init("Assets/Image/forward.dds", WIDTH, HEIGHT);
		break;
	case FixedReverse:
		m_sprite->Init("Assets/Image/reverse.dds", WIDTH, HEIGHT);
		break;
	}

	//表示場所を決定
	switch (m_side) {
	case Left:
		m_sprite->SetPosition(SPRITE_RIGHT);
		break;
	case Right:
		m_sprite->SetPosition(SPRITE_LEFT);
		break;
	case Up:
		m_sprite->SetPosition(SPRITE_UP);
		break;
	default:
		break;
	}

	//透明にしておく
	m_sprite->SetColor(SPRITE_INVISIBLE);
	//ピボットを設定
	m_sprite->SetPivot(SPRITE_PIVOT);
	m_sprite->SetScale(m_scale);

	return true;
}

void CycleDirection::ChangeSpriteForward()
{
	//回転方向が逆転のとき、正転のタイミングで正転スプライトに変更
	if (m_direction == Reverse) {
		m_sprite->Init("Assets/Image/forward_dir.dds", WIDTH, HEIGHT);
	}
}

void CycleDirection::ChangeSpriteReverse()
{
	//回転方向が正転のとき、逆転のタイミングで逆転スプライトに変更
	if (m_direction == Forward) {
		m_sprite->Init("Assets/Image/reverse_dir.dds", WIDTH, HEIGHT);
	}
}

void CycleDirection::ChangeFixedSpriteForward()
{
	if (m_direction == FixedReverse) {
		m_sprite->Init("Assets/Image/forward.dds", WIDTH, HEIGHT);
	}
}

void CycleDirection::ChangeFixedSpriteReverse()
{
	if (m_direction == FixedForward) {
		m_sprite->Init("Assets/Image/reverse.dds", WIDTH, HEIGHT);
	}
}

void CycleDirection::Update()
{
	const float SPRITE_CYCLE_SPEED = GameTime().GetFrameDeltaTime();
	/////////////////////////////////////////////////////
	//共通部分
	//ゲームプレイ中でなければUpdateを処理しない
	if (!GetGameDirector().GetIsGamePlay()) {
		return;
	}

	//開始時にフェードインする
	m_alpha += SPRITE_CYCLE_SPEED;
	if (m_alpha > 1.0f) {
		m_alpha = 1.0f;
	}
	m_finalColor.w = m_alpha;
	m_fixedColor.w = m_alpha;
	m_sprite->SetColor(m_finalColor);
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	//回転するスプライトのとき
	if (m_direction == Forward || m_direction == Reverse) {
		//回転角度修正
		if (m_direction == Forward) {
			m_angle += SPRITE_ANGLE_CHANGE_RATE;
			if (m_angle > 360.0f) {
				m_angle = 0.0f;
			}
		}
		else if (m_direction == Reverse) {
			m_angle -= SPRITE_ANGLE_CHANGE_RATE;
			if (m_angle < 0.0f) {
				m_angle = 360.0f;
			}
		}
		
	}
	/////////////////////////////////////////////////////

	/////////////////////////////////////////////////////
	//固定表示スプライトのとき
	if (m_direction == FixedForward || m_direction == FixedReverse) {
		m_sprite->SetColor(m_fixedColor);
		m_sprite->SetScale(FIXEDSPRITE_SCALE);
	}
	/////////////////////////////////////////////////////

	m_rotation.SetRotationDeg(Vector3::AxisZ, m_angle);

	m_sprite->SetRotation(m_rotation);
}

