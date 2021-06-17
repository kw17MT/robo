#include "stdafx.h"
#include "Score.h"
#include "Counter.h"
#include "FixedUI.h"
#include "FontRender.h"
#include "SoundSource.h"

namespace
{
	const int SCORE_POP_NUM = 2;
	const int SCORE_FOR_PLAYER_ONE = 0;
	const int SCORE_FOR_PLAYER_TWO = 1;

	const int SCORE_BASE_POINT = 100;
	const float SE_VOLUME = 0.7f;
}

Score::~Score()
{
	for (int i = 0; i < SCORE_POP_NUM; i++) {
		DeleteGO(m_score[i]);
	}
}

bool Score::Start()
{

	m_ui = FindGO<FixedUI>("ui");

	m_score[SCORE_FOR_PLAYER_ONE] = NewGO<FontRender>(5);
	m_score[SCORE_FOR_PLAYER_ONE]->SetText(L"0");
	m_score[SCORE_FOR_PLAYER_ONE]->SetPosition(m_score00Pos);

	m_score[SCORE_FOR_PLAYER_TWO] = NewGO<FontRender>(5);
	m_score[SCORE_FOR_PLAYER_TWO]->SetText(L"0");
	m_score[SCORE_FOR_PLAYER_TWO]->SetPosition(m_score01Pos);

	return true;
}

void Score::SetResult() {
	//引き分け
	if (m_score00 == m_score01) {
		ResultP1 = enResultP1_draw;
		ResultP2 = enResultP2_draw;
	}
	//プレイヤー1の勝利(=プレイヤー2の敗北)
	else if (m_score00 > m_score01) {
		ResultP1 = enResultP1_win;
		ResultP2 = enResultP2_lose;
	}
	//プレイヤー2の勝利(=プレイヤー1の敗北)
	else if (m_score00 < m_score01) {
		ResultP1 = enResultP1_lose;
		ResultP2 = enResultP2_win;
	}
}

void Score::AddScoreNChangeColor() {

	//プレイヤー１のためのもの
	m_score00 = m_basePoint00 * SCORE_BASE_POINT;
	m_nowScore00 = std::to_wstring(m_score00);
	//前フレームのスコアと現在のスコアが同じでないとき
	if (wcscmp(m_nowScore00.c_str(), m_prevScore00.c_str()) != 0) {
		Vector4 RED = { 1.0f,0.0f,0.0f,1.0f };
		//スコアの色を赤色にする。
		m_score[SCORE_FOR_PLAYER_ONE]->SetColor(RED);
		//前フレームのスコアを記録する。
		m_prevScore00 = m_nowScore00;
		//音を鳴らす。
		m_moneySound = NewGO<CSoundSource>(0);
		m_moneySound->Init(L"Assets/sound/Money.wav", false);
		m_moneySound->SetVolume(SE_VOLUME);
		m_moneySound->Play(false);
		//文字の色を赤色から徐々に白色にする準備をする。
		m_isChangeColor00 = true;
	}

	//現在のスコアを表示する。
	m_score[SCORE_FOR_PLAYER_ONE]->SetText(m_nowScore00.c_str());
	//スコアの色を変えなければならないならば
	if (m_isChangeColor00 == true) {
		//赤色以外の要素を少しずつ足していく。
		Vector4 addColorExceptRed = { 0.0f,0.01f,0.01f,0.0f };
		m_score[SCORE_FOR_PLAYER_ONE]->AddColorPoint(addColorExceptRed);
		//RBGすべての色が1.0f以上になったら色の変更をやめる。
		if (m_score[SCORE_FOR_PLAYER_ONE]->GetColorVolume() >= 1.0f) {
			m_isChangeColor00 = false;
		}
	}

	//プレイヤー２のためのもの
	m_score01 = m_basePoint01 * SCORE_BASE_POINT;
	m_nowScore01 = std::to_wstring(m_score01);
	//同じでないとき
	if (wcscmp(m_nowScore01.c_str(), m_prevScore01.c_str()) != 0) {
		Vector4 RED = { 1.0f,0.0f,0.0f,1.0f };
		m_score[SCORE_FOR_PLAYER_TWO]->SetColor(RED);
		m_prevScore01 = m_nowScore01;

		//音を鳴らす。
		m_moneySound = NewGO<CSoundSource>(0);
		m_moneySound->Init(L"Assets/sound/Money.wav", false);
		m_moneySound->SetVolume(SE_VOLUME);
		m_moneySound->Play(false);

		m_isChangeColor01 = true;
	}

	m_score[SCORE_FOR_PLAYER_TWO]->SetText(m_nowScore01.c_str());
	if (m_isChangeColor01 == true) {
		Vector4 addColorExceptRed = { 0.0f,0.01f,0.01f,0.0f };
		m_score[SCORE_FOR_PLAYER_TWO]->AddColorPoint(addColorExceptRed);
		if (m_score[SCORE_FOR_PLAYER_TWO]->GetColorVolume() >= 1.0f) {
			m_isChangeColor01 = false;
		}
	}
}


void Score::Update()
{
	if (m_ui->GetIsTimeUp() == true && m_isTimeUp == false) {
		//タイムアップ時、プレイヤーそれぞれに勝敗の状態を記録する
		SetResult();
		//score側のタイムアップフラグをtrueにする(判定を1回だけにする)
		SetIsTimeUp();
	}
	AddScoreNChangeColor();
}