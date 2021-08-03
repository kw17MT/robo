#include "stdafx.h"
#include "FixedUI.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "GameDirector.h"
#include "CycleDirection.h"

namespace
{
	const Vector2 TIME_TEXT_POS = { -100,350 };
	const Vector2 TIME_NUMBER_POS = { 50, 350 };
	const Vector2 SCORE_ONE_POS = { -600,-250 };
	const Vector2 SCORE_TWO_POS = { 250, -250 };
	const Vector2 MISS_TEXT_ONE_POS = { -600.0f,-300.0f };
	const Vector2 MISS_TEXT_TWO_POS = { 250.0f,-300.0f };
	const Vector2 TIME_NUM_PIVOT = { 0.5f,0.5f };

	
	const Vector4 MAKE_RED = { 1.0f, 0.0f, 0.0f, 1.0f };
	const Vector4 LITTLE_COLOR_EXCEPT_RED = { 0.0f,0.02f,0.02f,0.0f };

	const int SCORE_TEXT_NUM = 2;
	const int MISS_TEXT_NUM = 2;
	const int SCORE_LEFT = 0;
	const int SCORE_RIGHT = 1;
	const int MISS_LEFT = 0;
	const int MISS_RIGHT = 1;
	const float SCALE_BIGGER = 1.5f;
	const float SE_VOLUME = 0.7f;
	const float REDUCE_SCALE = -0.01f;
	const int TEN = 10;
}

//デストラクタ
FixedUI::~FixedUI()
{
	//TIMEの固定文字を消去
	DeleteGO(m_textTime);
	//SCOREの固定文字を消去
	for (int i = 0; i < SCORE_TEXT_NUM; i++) {
		DeleteGO(m_textScore[i]);
	}
	//可変の時間表示を消去
	DeleteGO(m_time);
	//MISSの固定文字を消去
	for (int i = 0; i < MISS_TEXT_NUM; i++) {
		DeleteGO(m_textMiss[i]);
	}

}

bool FixedUI::Start()
{
	//各種表示テキストと位置決め
	//時間(文字)
	m_textTime = NewGO<FontRender>(5);
	m_textTime->SetText(L"TIME : ");
	m_textTime->SetPosition(TIME_TEXT_POS);

	//スコア左(文字)
	m_textScore[SCORE_LEFT] = NewGO<FontRender>(5);
	m_textScore[SCORE_LEFT]->SetText(L"SCORE : ");
	m_textScore[SCORE_LEFT]->SetPosition(SCORE_ONE_POS);

	//スコア右(文字)
	m_textScore[SCORE_RIGHT] = NewGO<FontRender>(5);
	m_textScore[SCORE_RIGHT]->SetText(L"SCORE : ");
	m_textScore[SCORE_RIGHT]->SetPosition(SCORE_TWO_POS);

	//ミス表示
	m_textMiss[MISS_LEFT] = NewGO<FontRender>(5);
	m_textMiss[MISS_LEFT]->SetText(L"MISS:");
	m_textMiss[MISS_LEFT]->SetPosition(MISS_TEXT_ONE_POS);
	m_textMiss[MISS_RIGHT] = NewGO<FontRender>(5);
	m_textMiss[MISS_RIGHT]->SetText(L"MISS:");
	m_textMiss[MISS_RIGHT]->SetPosition(MISS_TEXT_TWO_POS);

	//残時間
	m_time = NewGO<FontRender>(5);
	m_time->SetPivot(TIME_NUM_PIVOT);

	std::wstring fontRemainingTime;
	fontRemainingTime = std::to_wstring(m_remainingTime);
	
	//残り時間を更新する。
	m_time->SetText(fontRemainingTime.c_str());

	m_time->SetPosition(TIME_NUMBER_POS);

	return true;
}

void FixedUI::RemainingTimeColor()
{
	if (m_remainingTime < TEN) {
		//最初10フレームは赤色にする
		if (m_timer < TEN) {
			m_time->SetScale(SCALE_BIGGER);
			m_time->SetColor(MAKE_RED);
		}
		//10フレーム後になれば赤色以外を少しずつ足して白色にする。
		if (m_timer >= TEN) {
			m_time->AddColorPoint(LITTLE_COLOR_EXCEPT_RED);
			m_time->AddFontScale(REDUCE_SCALE);
		}
	}
}

void FixedUI::Update()
{
	//ゲームプレイ中じゃなかったら。
	if (!GetGameDirector().GetIsGamePlay())
	{
		//処理しない。
		return;
	}

	//タイム減少とタイムアップ処理
	//変数timerの値が60になる度に残時間remainingTimeから1を引いていく
	//TODO ゲーム内の時間を計ってる。
	float gameTime = GameTime().GetFrameDeltaTime();

	m_remainingTime -= gameTime / 2.0f;
	//残り時間が10秒未満の時
	if (m_remainingTime < TEN) {
		//音を出す。
		m_timeSound = NewGO<CSoundSource>(0);
		m_timeSound->Init(L"Assets/sound/Time.wav", false);
		m_timeSound->SetVolume(SE_VOLUME);
		m_timeSound->Play(false);
	}

	////タイムアップフラグを立てる
	if (m_remainingTime <= 0 && m_isTimeUp == false) {
		m_isTimeUp = true;
	}

	//残時間の変換と更新(int → wstring → const wchar_t*)
	//残時間LastTimeをstd::wstring型の文字列に変換する
	std::wstring fontRemainingTime;
	fontRemainingTime = std::to_wstring(static_cast<int>(m_remainingTime));
	//残り時間を更新する。
	m_time->SetText(fontRemainingTime.c_str());

	//残時間が少ないときの演出
	//残り10秒未満になると拡大表示→縮小、色を赤色から白色に変えて強調表示。
	if (m_remainingTime > 0) {
		RemainingTimeColor();
	}
}