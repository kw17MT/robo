#include "stdafx.h"
#include "Score.h"
#include "Counter.h"
#include "FixedUI.h"
#include "FontRender.h"

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


void Score::Update()
{
	
	if (m_ui->GetIsTimeUp() == true && GetIsTimeUp() == false) {
		//タイムアップ時、プレイヤーそれぞれに勝敗の状態を記録する
		SetResult();
		//score側のタイムアップフラグをtrueにする(判定を1回だけにする)
		SetIsTimeUp();
	}


	//０１は１Pの、０２は２Pのスコア
	Font score01;
	Font score02;
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };

	Score01 = BasePoint01 * 100;
	std::wstring str01;
	str01 = std::to_wstring(Score01);

	Score02 = BasePoint02 * 100;
	std::wstring str02;
	str02 = std::to_wstring(Score02);

	Score[0]->SetText(str01.c_str());
	Score[1]->SetText(str02.c_str());

	/*score01.Begin(renderContext);
	score01.Draw(str01.c_str(), { -400.0f,-300.0f }, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	score01.End(renderContext);

	score02.Begin(renderContext);
	score02.Draw(str02.c_str(), { 550.0f,-300.0f }, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	score02.End(renderContext);*/
}