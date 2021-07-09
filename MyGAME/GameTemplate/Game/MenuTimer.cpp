#include "stdafx.h"
#include "MenuTimer.h"
#include "SkinModelRender.h"
#include "GameDirector.h"


namespace
{
	const Vector3 MAKE_MODEL_GREEN = { 0.0f, 100.0f, 0.0f };

	const float DEFAULT_TIMER_SCALE_X = 2.8f;
	const float TIMER_TURN_YELLOW_POINT = 0.9f;
	const float MAKE_YELLOW_AMOUNT = 0.001f;
	const float MAKE_RED_AMOUNT = 0.4f;
	const float TIMER_DECREASE_RATE = 0.001f;
	const float GOOD_AMOUNT_OF_RED = 2.0f;
	const float TIMER_TURN_RED_POINT = 1.0f;
}
MenuTimer::~MenuTimer()
{
	DeleteGO(m_skinModelRender);
}

bool MenuTimer::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画用モデルの初期化
	m_skinModelRender->Init("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position);
	//モデルの向きの調節
	m_skinModelRender->SetRotation(m_rot);
	return true;
}

void MenuTimer::ResetTimerParam()
{
	//拡大率を初期の状態にする。
	m_scale.x = DEFAULT_TIMER_SCALE_X;
	//緑色にする
	//m_lig.ambientLight.Set(MAKE_MODEL_GREEN);
	m_isTimeUp = false;
}


void MenuTimer::Update()
{
	//ゲームがゲームが開始されていなければ
	if (GetGameDirector().GetGameScene() != enGamePlay) {
		//処理しない。
		return;
	}

	//毎フレームタイマーの拡大率をXのみ下げていく。
	m_scale.x -= TIMER_DECREASE_RATE ;
	//timer--;

	//ゲージの大きさが一定異以上であるとき赤成分を足す = 徐々に黄色にしていく
	if (m_scale.x >= TIMER_TURN_YELLOW_POINT) {
		//m_lig.ambientLight.x += MAKE_YELLOW_AMOUNT;
		//赤色が丁度良くなったら
		/*if (m_lig.ambientLight.x >= GOOD_AMOUNT_OF_RED) {
			m_lig.ambientLight.x = GOOD_AMOUNT_OF_RED;
		}*/
	}
	//ゲージの色を赤くしたいポイントまで縮小したら
	if (m_scale.x < TIMER_TURN_RED_POINT) {
		//ひたすら、緑色成分を抜いていく。
		//m_lig.ambientLight.y -= MAKE_RED_AMOUNT;
		//if (m_lig.ambientLight.y <= 0.0f) {
		//	m_lig.ambientLight.y = 0.0f;
		//}
	}

	//タイマーの縮尺が0になったら＝残量が消えたら
	if (m_scale.x <= 0.0f) {
		m_isTimeUp = true;
	}

	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRotation(m_rot);
	m_skinModelRender->SetPosition(m_position);
}