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

	//タイム減少とタイムアップ処理
	//変数timerの値が60になる度に残時間LastTimeから1を引いていく
	//Font Timer;
	timer++;
	if (timer >= 60) {
		if (LastTime > 0) {
			LastTime--;
		}
		timer = 0;
	}
	//タイムアップフラグを立てる
	if (LastTime <= 0 && isTimeUp == false) {
		isTimeUp = true;
	}

	//タイムアップフラグ表示
	//文字表示
	fontTime.Begin(renderContext);
	fontTime.Draw(L"TIMEUP :", posTimeUpText, color, 0.0f, 1.0f, { 0.0f,0.0f });
	fontTime.End(renderContext);
	//状態表示
	const wchar_t* timeUpStateWc;
	if (isTimeUp == true) {
		timeUpStateWc = L"ON";
	}
	else {
		timeUpStateWc = L"OFF";
	}
	fontTime.Begin(renderContext);
	fontTime.Draw(timeUpStateWc, posTimeUpState, color, 0.0f, 1.0f, { 0.0f,0.0f });
	fontTime.End(renderContext);
	


	//残時間LastTimeをstd::wstring型の文字列に変換する
	std::wstring fontLastTime;
	fontLastTime = std::to_wstring(LastTime);

	//残時間の表示
	fontTime.Begin(renderContext);
	fontTime.Draw(fontLastTime.c_str(), posLastTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);

}