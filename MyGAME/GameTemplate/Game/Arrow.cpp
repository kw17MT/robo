#include "stdafx.h"
#include "Arrow.h"
#include "SkinModelRender.h"

namespace 
{
	const int TIMER_MAX = 50;
	const int TIMER_MIDDLE = 25;
	const float CHANGE_VOLUME_POS = 1.0f;
	const Vector3 CHANGE_VOLUME_SCALE = { 0.1f, 0.1f, 0.1f };
}

Arrow::~Arrow()
{
	DeleteGO(m_skinModelRender);
}

bool Arrow::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常モデルの初期化
	m_skinModelRender->Init("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_position);
	//シャドウキャスト用の初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_position);

	return true;
}

void Arrow::Float()
{
	m_timer++;
	//y座標を上にしていく
	if (m_timer < TIMER_MIDDLE) {
		m_position.y += CHANGE_VOLUME_POS;
	}
	//y座標を下にしていく
	if (m_timer >= TIMER_MIDDLE && m_timer <= TIMER_MAX) {
		m_position.y -= CHANGE_VOLUME_POS;
	}
	//タイマーを０にする。
	if(m_timer > TIMER_MAX) {
		m_timer = 0;
	}
}

void Arrow::Expansion()
{
	//自身の拡大率は最小でないことを設定する。
	m_scaleMinFlag = false;
	//拡大していく。
	m_scale += CHANGE_VOLUME_SCALE;

	//拡大率のどれかが１以上になれば、すべての拡大率を１にする。
	if (m_scale.x > 1.0f || m_scale.y > 1.0f || m_scale.z > 1.0f) {
		m_scale = Vector3::One;
		m_scaleMaxFlag = true;
	}
}

void Arrow::Shrink()
{
	//自身の拡大率は最大でないことを設定する。
	m_scaleMaxFlag = false;
	//縮小していく
	m_scale -= CHANGE_VOLUME_SCALE;

	//拡大率のどれかが０未満になれば、すべての拡大率を０にする。
	if (m_scale.x < 0.0f || m_scale.y < 0.0f || m_scale.z < 0.0f) {
		m_scale = Vector3::Zero;
		m_scaleMinFlag = true;
	}
}

void Arrow::Update()
{
	//浮遊処理
	Float();
	//位置設定
	m_skinModelRender->SetPosition(m_position);
	//拡大率設定
	m_skinModelRender->SetScale(m_scale);
}