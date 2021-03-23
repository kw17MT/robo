#include "stdafx.h"
#include "FixedUI.h"

void FixedUI::Update()
{
	Font fontTime;
	//カラー
	Vector4 color = { 1.0f,1.0f,1.0,1.0f };
	//TIMEの文字
	fontTime.Begin(renderContext);
	fontTime.Draw(L"TIME :", posTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);
	//プレイヤー1のITEM文字
	fontTime.Begin(renderContext);
	fontTime.Draw(L"ITEM", posItem01, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);
	//プレイヤー2のITEM文字
	fontTime.Begin(renderContext);
	fontTime.Draw(L"ITEM", posItem02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);
	//プレイヤー1のSCORE文字
	fontTime.Begin(renderContext);
	fontTime.Draw(L"SCORE :", posScore01, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);
	//プレイヤー2のSCORE文字
	fontTime.Begin(renderContext);
	fontTime.Draw(L"SCORE :", posScore02, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

	//タイム減少処理
	//変数timerの値が60になる度に残時間LastTimeから1を引いていく
	//Font Timer;
	timer++;
	if (timer == 60) {
		if (LastTime > 0) {
			LastTime--;
		}
		timer = 0;
	}

	


	//残時間LastTimeをstd::wstring型の文字列に変換する
	std::wstring fontLastTime;
	fontLastTime = std::to_wstring(LastTime);

	//残時間の表示
	fontTime.Begin(renderContext);
	fontTime.Draw(fontLastTime.c_str(), posLastTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

}