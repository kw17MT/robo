#include "stdafx.h"
#include "FixedUI.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "GameDirector.h"
#include "CycleDirection.h"

//デストラクタ
FixedUI::~FixedUI()
{
	DeleteGO(TextTime);
	//DeleteGO(TextTimeUp);
	for (int i = 0; i < 2; i++) {
		DeleteGO(TextScore[i]);
	}
	//for (int i = 0; i < 2; i++) {
	//	DeleteGO(TextItem[i]);
	//}
	DeleteGO(Time);
	//DeleteGO(isTimeUpState);
	for (int i = 0; i < 2; i++) {
		DeleteGO(TextMiss[i]);
	}

	/*DeleteGO(convDirText);
	DeleteGO(convDirLeft);
	DeleteGO(convDirRight);*/

}

bool FixedUI::Start()
{
	//各種表示テキストと位置決め
	//時間(文字)
	//TextTime = NewGO<FontRender>(5);
	//TextTime->SetText(L"TIME : ");
	//TextTime->SetPosition(posTime);

	//時間切れフラグ(文字)
	/*TextTimeUp = NewGO<FontRender>(5);
	TextTimeUp->SetText(L"TIMEUP : ");
	TextTimeUp->SetPosition(posTimeUpText);*/

	//スコア左(文字)
	TextScore[0] = NewGO<FontRender>(5);
	TextScore[0]->SetText(L"SCORE : ");
	TextScore[0]->SetPosition(posScore01);

	//スコア右(文字)
	TextScore[1] = NewGO<FontRender>(5);
	TextScore[1]->SetText(L"SCORE : ");
	TextScore[1]->SetPosition(posScore02);

	//アイテム左(文字)
	/*TextItem[0] = NewGO<FontRender>(5);
	TextItem[0]->SetText(L"ITEM : ");
	TextItem[0]->SetPosition(posItem01);*/

	//アイテム右(文字)
	/*TextItem[1] = NewGO<FontRender>(5);
	TextItem[1]->SetText(L"ITEM : ");
	TextItem[1]->SetPosition(posItem02);*/

	//タイムアップ状態
	/*isTimeUpState = NewGO<FontRender>(5);
	isTimeUpState->SetText(TurnTimeUpState());
	isTimeUpState->SetPosition(posTimeUpState);*/

	//ミス表示
	TextMiss[0] = NewGO<FontRender>(5);
	TextMiss[0]->SetText(L"MISS:");
	TextMiss[0]->SetPosition(posMiss01);
	TextMiss[1] = NewGO<FontRender>(5);
	TextMiss[1]->SetText(L"MISS:");
	TextMiss[1]->SetPosition(posMiss02);

	////コンベア回転方向表示
	//convDirText = NewGO<SpriteRender>(0);
	//convDirText->Init("Assets/Image/forward.dds", 512*0.25, 512*0.25);
	//convDirText->SetPivot({ 0.5f,0.5f });
	//convDirText->SetPosition(posConvDirText);
	//
	//convDirLeft = NewGO<SpriteRender>(0);
	//convDirLeft->Init("Assets/Image/forward_dir.dds", 512*0.5, 512*0.5);
	//convDirLeft->SetPivot({ 0.5f,0.5f });
	//convDirLeft->SetPosition(posConvDirLeft);
	//
	//convDirRight = NewGO<SpriteRender>(0);
	//convDirRight->Init("Assets/Image/reverse_dir.dds", 512*0.5, 512*0.5);
	//convDirRight->SetPivot({ 0.5f,0.5f });
	//convDirRight->SetPosition(posConvDirRight);

	//残時間
	//Time = NewGO<FontRender>(5);

	//std::wstring fontRemainingTime;
	//fontRemainingTime = std::to_wstring(remainingTime);
	////残り時間を更新する。
	//Time->SetText(fontRemainingTime.c_str());

	//Time->SetPosition(posLastTime);

	/*m_directionSprite[0] = NewGO<CycleDirection>(0);
	m_directionSprite[0]->SetDirection(Forward);
	m_directionSprite[0]->SetSide(Left);
	m_directionSprite[1] = NewGO<CycleDirection>(0);
	m_directionSprite[0]->SetDirection(Reverse);
	m_directionSprite[0]->SetSide(Right);*/

	return true;
}

//自身のタイムアップフラグを調べて、
//タイムアップフラグがOFFならOFF,ONならON
//という文字列(const wchar_t*型)を返す
//const wchar_t* FixedUI::TurnTimeUpState()
//{
//	const wchar_t* timeUpStateWc;
//	if (isTimeUp == true) {
//		timeUpStateWc = L"ON";
//	}
//	else {
//		timeUpStateWc = L"OFF";
//	}
//
//	return timeUpStateWc;
//}

void FixedUI::RemainingTimeColor()
{
	/*if (remainingTime < 10) {
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
	}*/
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
	//timer++;
	//if (timer >= 60) {
	//	if (remainingTime > 0) {
	//		remainingTime--;
	//	}
	//	if (remainingTime < 10) {
	//		//音を出す。
	//		timeSound = NewGO<CSoundSource>(0);
	//		timeSound->Init(L"Assets/sound/Time.wav", false);
	//		timeSound->SetVolume(0.7f);
	//		timeSound->Play(false);
	//	}
	//	timer = 0;
	//}
	//////タイムアップフラグを立てる
	//if (remainingTime <= 0 && isTimeUp == false) {
	//	isTimeUp = true;
	//}
	//タイムアップしたかしてないかで文字を変更
	//isTimeUpState->SetText(TurnTimeUpState());
	//タイムアップ処理終わり

	//残時間の変換と更新(int → wstring → const wchar_t*)
	//残時間LastTimeをstd::wstring型の文字列に変換する
	//std::wstring fontRemainingTime;
	//fontRemainingTime = std::to_wstring(remainingTime);
	////残り時間を更新する。
	//Time->SetText(fontRemainingTime.c_str());

	//残時間が少ないときの演出
	//残り10秒未満になると拡大表示→縮小、色を赤色から白色に変えて強調表示。
	/*if (remainingTime > 0) {
	RemainingTimeColor();

	
<<<<<<< HEAD
	}
=======
	}*/

	
	////コンベア回転方向表示処理
	//angleLeft += 120.0f / 60.0f;
	//angleRight -= 120.0f / 60.0f;
	
	////コンベア回転方向表示処理
	//angleLeft += 120.0f / 60.0f;
	//angleRight -= 120.0f / 60.0f;
	//

	////回転角度を0～360度にする
	//if (angleLeft > 360.0f) {
	//	angleLeft = 0.0f;
	//}
	//if (angleRight > 360.0f) {
	//	angleRight = 0.0f;
	//}

	//rotLeft.SetRotationDeg(Vector3::AxisZ, angleLeft);
	//rotRight.SetRotationDeg(Vector3::AxisZ, angleRight);

	//convDirLeft->SetRotation(rotLeft);
	//convDirRight->SetRotation(rotRight);
	
}