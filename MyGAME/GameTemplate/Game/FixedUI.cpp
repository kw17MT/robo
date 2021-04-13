#include "stdafx.h"
#include "FixedUI.h"
#include "FontRender.h"
#include "SoundSource.h"

#include "GameDirector.h"

//デストラクタ
FixedUI::~FixedUI()
{
	DeleteGO(TextTime);
	DeleteGO(TextTimeUp);
	for (int i = 0; i < 2; i++) {
		DeleteGO(TextScore[i]);
	}
	for (int i = 0; i < 2; i++) {
		DeleteGO(TextItem[i]);
	}
	DeleteGO(Time);
	DeleteGO(isTimeUpState);
}

bool FixedUI::Start()
{
	//各種表示テキストと位置決め
	//時間(文字)
	TextTime = NewGO<FontRender>(5);
	TextTime->SetText(L"TIME : ");
	TextTime->SetPosition(posTime);

	//時間切れフラグ(文字)
	TextTimeUp = NewGO<FontRender>(5);
	TextTimeUp->SetText(L"TIMEUP : ");
	TextTimeUp->SetPosition(posTimeUpText);

	//スコア左(文字)
	TextScore[0] = NewGO<FontRender>(5);
	TextScore[0]->SetText(L"SCORE : ");
	TextScore[0]->SetPosition(posScore01);

	//スコア右(文字)
	TextScore[1] = NewGO<FontRender>(5);
	TextScore[1]->SetText(L"SCORE : ");
	TextScore[1]->SetPosition(posScore02);

	//アイテム左(文字)
	TextItem[0] = NewGO<FontRender>(5);
	TextItem[0]->SetText(L"ITEM : ");
	TextItem[0]->SetPosition(posItem01);

	//アイテム右(文字)
	TextItem[1] = NewGO<FontRender>(5);
	TextItem[1]->SetText(L"ITEM : ");
	TextItem[1]->SetPosition(posItem02);

	//タイムアップ状態
	isTimeUpState = NewGO<FontRender>(5);
	isTimeUpState->SetText(TurnTimeUpState());
	isTimeUpState->SetPosition(posTimeUpState);

	//残時間
	Time = NewGO<FontRender>(5);
	Time->SetText(L"60");
	Time->SetPosition(posLastTime);

	return true;
}

//自身のタイムアップフラグを調べて、
//タイムアップフラグがOFFならOFF,ONならON
//という文字列(const wchar_t*型)を返す
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

void FixedUI::RemainingTimeColor()
{
	if (remainingTime < 10) {
		if (timer < 10) {
			const float Scale = 1.5f;
			const Vector4 RED = { 1.0f, 0.0f, 0.0f, 1.0f };
			Time->SetScale(Scale);
			Time->SetColor(RED);
		}
		if (timer >= 10) {
			const float reduceScale = -0.01f;
			const Vector4 addColorExceptRed = { 0.0f,0.02f,0.02f,0.0f };
			Time->AddColorPoint(addColorExceptRed);
			Time->AddFontScale(reduceScale);
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

	//Font fontTime;
	//カラー
	//Vector4 color = { 1.0f,1.0f,1.0,1.0f };


	//タイム減少とタイムアップ処理
	//変数timerの値が60になる度に残時間remainingTimeから1を引いていく
	//TODO ゲーム内の時間を計ってる。
	timer++;
	if (timer >= 60) {
		if (remainingTime > 0) {
			remainingTime--;
		}
		if (remainingTime < 10) {
			//音を出す。
			timeSound = NewGO<CSoundSource>(0);
			timeSound->Init(L"Assets/sound/Time.wav", false);
			timeSound->SetVolume(0.7f);
			timeSound->Play(false);
		}
		timer = 0;
	}
	//タイムアップフラグを立てる
	if (remainingTime <= 0 && isTimeUp == false) {
		isTimeUp = true;
	}
	//タイムアップしたかしてないかで文字を変更
	isTimeUpState->SetText(TurnTimeUpState());
	//タイムアップ処理終わり

	//残時間の変換と更新(int → wstring → const wchar_t*)
	//残時間LastTimeをstd::wstring型の文字列に変換する
	std::wstring fontRemainingTime;
	fontRemainingTime = std::to_wstring(remainingTime);
	//残り時間を更新する。
	Time->SetText(fontRemainingTime.c_str());

	//残時間が少ないときの演出
	//残り10秒未満になると拡大表示→縮小、色を赤色から白色に変えて強調表示。
	if (remainingTime > 0) {
	RemainingTimeColor();

	
	}
}