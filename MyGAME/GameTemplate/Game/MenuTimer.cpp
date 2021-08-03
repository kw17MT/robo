#include "stdafx.h"
#include "MenuTimer.h"
#include "SkinModelRender.h"
#include "GameDirector.h"
#include "LightManager.h"


namespace
{
	const float DEFAULT_TIMER_SCALE_X = 2.8f;
	const float TIMER_TURN_YELLOW_POINT = 1.7f;
	const float TIMER_TURN_RED_POINT = 0.9f;
	const float TIMER_DECREASE_RATE = 0.001f;
	const float STIRRING_AMOUNT = 0.03f;
	const float MAX_STIRRING = 1.2f;
	const float MIN_STIRRING = 1.0f;
}

MenuTimer::~MenuTimer()
{
	DeleteGO(m_skinModelRender);
}

bool MenuTimer::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画用モデルの初期化
	m_skinModelRender->InitAsGauge("Assets/modelData/gauge/gauge_red.tkm", nullptr, enModelUpAxisY, m_position, m_gaugeNumber);
	//モデルの向きの調節
	m_skinModelRender->SetRotation(m_rot);
	return true;
}

void MenuTimer::ResetTimerParam(int lightNumber)
{
	//拡大率を初期の状態にする。
	m_scale.x = DEFAULT_TIMER_SCALE_X;
	//赤色に設定したことをなかったことに
	m_completedMakeRed = false;
	//黄色に設定したことをなかったことに
	m_completedMakeYellow = false;
	//ゲージを緑色に設定しなおす
	LightManager::GetInstance().MakeGaugeLightGreen(lightNumber);
	//ブルームをかけないようにする
	m_skinModelRender->SetApplyBlur(false);
	//タイムアップフラグを初期値に戻す
	m_isTimeUp = false;
}

void MenuTimer::StirringGauge()
{
	//上に鼓動させる
	if (m_stirringBigger) {
		m_scale.z += STIRRING_AMOUNT;
		if (m_scale.z >= MAX_STIRRING) m_stirringBigger = false;
	}
	//鼓動を元に戻す
	if (!m_stirringBigger) {
		m_scale.z -= STIRRING_AMOUNT;
		if (m_scale.z <= MIN_STIRRING) m_stirringBigger = true;
	}
}

void MenuTimer::Update()
{
	//ゲームがゲームが開始されていなければ
	if (GetGameDirector().GetGameScene() != enGamePlay) {
		//処理しない。
		return;
	}

	//毎フレームタイマーの拡大率をXのみ下げていく。
	m_scale.x -= TIMER_DECREASE_RATE;

	//ゲージの大きさが一定以下になると黄色にする
	if (m_scale.x <= TIMER_TURN_YELLOW_POINT && !m_completedMakeYellow) {
		//ゲージ専用ライトを黄色にする
		LightManager::GetInstance().MakeGaugeLightYellow(m_gaugeNumber);
		//黄色にしたことを記録
		m_completedMakeYellow = true;
	}
	//ゲージの大きさが一定以下になると赤色にする
	if (m_scale.x <= TIMER_TURN_RED_POINT && !m_completedMakeRed) {
		//強調表示のためにブルームをかける
		m_skinModelRender->SetApplyBlur(true);
		//強調表示のために鼓動させる
		StirringGauge();
		//ゲージ専用ライトを赤色にする
		LightManager::GetInstance().MakeGaugeLightRed(m_gaugeNumber);
	}

	//タイマーの縮尺が0になったら＝残量が消えたら
	if (m_scale.x <= 0.0f) {
		//タイムアップしたことを記録
		m_isTimeUp = true;
		//ブルームをしないようにする
		m_skinModelRender->SetApplyBlur(false);
	}
	//回転の更新
	m_skinModelRender->SetRotation(m_rot);
	//拡大率の更新
	m_skinModelRender->SetScale(m_scale);
	//位置の更新
	m_skinModelRender->SetPosition(m_position);
}