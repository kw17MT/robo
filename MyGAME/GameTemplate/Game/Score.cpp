#include "stdafx.h"
#include "Score.h"
#include "Counter.h"
#include "FixedUI.h"
#include "FontRender.h"
#include "SoundSource.h"

bool Score::Start()
{

	m_ui = FindGO<FixedUI>("ui");

	Score[0] = NewGO<FontRender>(5);
	Score[0]->SetText(L"0");
	Score[0]->SetPosition(Score01Pos);

	Score[1] = NewGO<FontRender>(5);
	Score[1]->SetText(L"0");
	Score[1]->SetPosition(Score02Pos);

	return true;
}

void Score::SetResult() {
	//引き分け
	if (Score01 == Score02) {
		ResultP1 = enResultP1_draw;
		ResultP2 = enResultP2_draw;
	}
	//プレイヤー1の勝利(=プレイヤー2の敗北)
	else if (Score01 > Score02) {
		ResultP1 = enResultP1_win;
		ResultP2 = enResultP2_lose;
	}
	//プレイヤー2の勝利(=プレイヤー1の敗北)
	else if (Score01 < Score02) {
		ResultP1 = enResultP1_lose;
		ResultP2 = enResultP2_win;
	}
}

void Score::AddScoreNChangeColor() {

	//プレイヤー１のためのもの
	Score01 = BasePoint01 * 100;
	nowScore01 = std::to_wstring(Score01);
	//前フレームのスコアと現在のスコアが同じでないとき
	if (wcscmp(nowScore01.c_str(), prevScore01.c_str()) != 0) {
		Vector4 RED = { 1.0f,0.0f,0.0f,1.0f };
		//スコアの色を赤色にする。
		Score[0]->SetColor(RED);
		//前フレームのスコアを記録する。
		prevScore01 = nowScore01;
		//音を鳴らす。
		moneySound = NewGO<CSoundSource>(0);
		moneySound->Init(L"Assets/sound/Money.wav", false);
		moneySound->SetVolume(0.7f);
		moneySound->Play(false);
		//文字の色を赤色から徐々に白色にする準備をする。
		isChangeColor01 = true;
	}

	//現在のスコアを表示する。
	Score[0]->SetText(nowScore01.c_str());
	//スコアの色を変えなければならないならば
	if (isChangeColor01 == true) {
		//赤色以外の要素を少しずつ足していく。
		Vector4 addColorExceptRed = { 0.0f,0.01f,0.01f,0.0f };
		Score[0]->AddColorPoint(addColorExceptRed);
		//RBGすべての色が1.0f以上になったら色の変更をやめる。
		//operator==
		if (Score[0]->GetColorVolume() >= 1.0f) {
			isChangeColor01 = false;
		}
	}

	//プレイヤー２のためのもの
	Score02 = BasePoint02 * 100;
	nowScore02 = std::to_wstring(Score02);
	//同じでないとき
	if (wcscmp(nowScore02.c_str(), prevScore02.c_str()) != 0) {
		Vector4 RED = { 1.0f,0.0f,0.0f,1.0f };
		Score[1]->SetColor(RED);
		prevScore02 = nowScore02;

		//音を鳴らす。
		moneySound = NewGO<CSoundSource>(0);
		moneySound->Init(L"Assets/sound/Money.wav", false);
		moneySound->SetVolume(0.7f);
		moneySound->Play(false);

		isChangeColor02 = true;
	}

	Score[1]->SetText(nowScore02.c_str());
	if (isChangeColor02 == true) {
		Vector4 addColorExceptRed = { 0.0f,0.01f,0.01f,0.0f };
		Score[1]->AddColorPoint(addColorExceptRed);
		if (Score[1]->GetColorVolume() >= 1.0f) {
			isChangeColor02 = false;
		}
	}
}


void Score::Update()
{
	if (m_ui->GetIsTimeUp() == true && GetIsTimeUp() == false) {
		//タイムアップ時、プレイヤーそれぞれに勝敗の状態を記録する
		SetResult();
		//score側のタイムアップフラグをtrueにする(判定を1回だけにする)
		SetIsTimeUp();
	}

	AddScoreNChangeColor();
}