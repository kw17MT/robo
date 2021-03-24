#include "stdafx.h"
#include "FixedUI.h"
#include "FontRender.h"


const wchar_t* FixedUI::TurnTimeUpState()
{
	const wchar_t* timeUpStateWc;
	if (isTimeUp == true) {
		timeUpStateWc = L"ON";
	}
	else {
		timeUpStateWc = L"OFF";
	}

	return timeUpStateWc;
}

bool FixedUI::Start()
{
	TextTime = NewGO<FontRender>(5);
	TextTime->SetText(L"TIME : ");
	TextTime->SetPosition(posTime);

	TextTimeUp = NewGO<FontRender>(5);
	TextTimeUp->SetText(L"TIMEUP : ");
	TextTimeUp->SetPosition(posTimeUpText);

	TextScore[0] = NewGO<FontRender>(5);
	TextScore[0]->SetText(L"SCORE : ");
	TextScore[0]->SetPosition(posScore01);

	TextScore[1] = NewGO<FontRender>(5);
	TextScore[1]->SetText(L"SCORE : ");
	TextScore[1]->SetPosition(posScore02);

	TextItem[0] = NewGO<FontRender>(5);
	TextItem[0]->SetText(L"ITEM : ");
	TextItem[0]->SetPosition(posItem01);

	TextItem[1] = NewGO<FontRender>(5);
	TextItem[1]->SetText(L"ITEM : ");
	TextItem[1]->SetPosition(posItem02);

	isTimeUpState = NewGO<FontRender>(5);
	isTimeUpState->SetText(TurnTimeUpState());
	isTimeUpState->SetPosition(posTimeUpState);

	Time = NewGO<FontRender>(5);
	Time->SetText(L"60");
	Time->SetPosition(posLastTime);

	return true;
}

void FixedUI::Update()
{
	//Font fontTime;
	//カラー
	//Vector4 color = { 1.0f,1.0f,1.0,1.0f };


	//タイム減少とタイムアップ処理
	//変数timerの値が60になる度に残時間LastTimeから1を引いていく


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
	//fontTime.Begin(renderContext);
	//fontTime.Draw(L"TIMEUP :", posTimeUpText, color, 0.0f, 1.0f, { 0.0f,0.0f });
	//fontTime.End(renderContext);
	//
	////状態表示
	//fontTime.Begin(renderContext);
	//fontTime.Draw(TurnTimeUpState(), posTimeUpState, color, 0.0f, 1.0f, { 0.0f,0.0f });
	//fontTime.End(renderContext);
	//

	//残時間LastTimeをstd::wstring型の文字列に変換する
	std::wstring fontLastTime;
	fontLastTime = std::to_wstring(LastTime);
	//残り時間を更新する。
	Time->SetText(fontLastTime.c_str());

	//残時間の表示
	/*fontTime.Begin(renderContext);
	fontTime.Draw(fontLastTime.c_str(), posLastTime, color, 0.0f, 1.0f, { 0.0f, 0.0f });
	fontTime.End(renderContext);*/

}