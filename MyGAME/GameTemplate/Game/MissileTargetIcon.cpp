#include "stdafx.h"
#include "MissileTargetIcon.h"
#include "SpriteRender.h"
#include "Enemy.h"

namespace
{
	const int ICON_SIZE = 96;									//アイコンの大きさ
	const Vector3 EXPANDED_SCALE = { 2.0f,2.0f,1.0f };			//拡大した時の大きさ
	const Vector3 SHRINK_RATE = { 0.1f, 0.1f, 0.0f };			//縮小速度
	const float DECREASE_ANGLE_RATE = 9.0f;						//角度の減少速度
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

MissileTargetIcon::~MissileTargetIcon()
{
	//アイコン画像を削除
	DeleteGO(m_spriteRender);
}

bool MissileTargetIcon::Start()
{
	//アイコン生成
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/reticle/reticle_missile.dds", ICON_SIZE, ICON_SIZE);

	return true;
}

void MissileTargetIcon::Update()
{
	//敵のスクリーン座標を計算する
	CalcMethods::CalcScreenPos(m_screenPos, m_enemy->GetPosition());

	//アイコンを出した時は拡大した状態で出す。
	if (m_isFirstExpand)
	{
		//拡大
		m_scale = EXPANDED_SCALE;
		//90度回転させた状態にする
		m_angle = 90.0f;
		//拡大を行わないように
		m_isFirstExpand = false;
	}
	//縮小させていく
	m_scale -= SHRINK_RATE;
	//角度を減らしていく
	m_angle -= DECREASE_ANGLE_RATE;

	if (m_scale.x <= 1.0f)
	{
		m_scale = Vector3::One;
	}
	if (m_angle <= 0.0f)
	{
		m_angle = 0.0f;
	}

	//回転を作成
	qRot.SetRotationDegZ(m_angle);
	//画像を回転させる
	m_spriteRender->SetRotation(qRot);
	//画像の位置を更新
	m_spriteRender->SetPosition(m_screenPos);
	//画像の拡大率を更新
	m_spriteRender->SetScale(m_scale);
}