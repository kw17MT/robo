#include "stdafx.h"
#include "Result.h"
#include "SpriteRender.h"
#include "FixedUI.h"

Result::~Result()
{
	DeleteGO(this);
}

void Result::DecideDDS()
{
	//spriteNumの値に応じて初期化に使用するDDSを振り分ける
	switch (spriteNum) {
	case 0: //引き分け
		m_spriteRender = NewGO<SpriteRender>(1);
		m_spriteRender->Init("Assets/Image/draw.dds", wideth*0.75f, height*0.75f);
		break;
	case 1: //勝利
		m_spriteRender = NewGO<SpriteRender>(1);
		m_spriteRender->Init("Assets/Image/win.dds", wideth*0.75f, height*0.75f);
		break;
	case 2: //敗北
		m_spriteRender = NewGO<SpriteRender>(1);
		m_spriteRender->Init("Assets/Image/lose.dds", wideth*0.5f, height*0.5f);
		break;
	default:
		break;
	}
}

void Result::DecidePos()
{
	//positionNumの値に応じて表示位置を振り分ける
	switch (positionNum) {
	case 0: //引き分け : 画面中央くらい
		m_spriteRender->SetPosition(center);
		break;
	case 1: //左側
		m_spriteRender->SetPosition(left);
		break;
	case 2://右側
		m_spriteRender->SetPosition(right);
		break;
	default:
		break;
	}
}

bool Result::Start()
{
	//中央、左、右の画像表示位置用変数を設定
	center.Set(0.0f,0.0f,0.0f);
	left.Set(0.0f + 450.0f, 0.0f,0.0f);
	right.Set(0.0f - 450.0f, 0.0f,0.0f);

	//UIを検索
	m_ui = FindGO<FixedUI>("ui");

	if (m_ui->GetIsTimeUp() && GetIsTimeUp() == false) {
		//読み込むddsを決定
		DecideDDS();
		//スプライトを表示する位置を決定
		DecidePos();
		//タイムアップフラグを立てる
		SetIsTimeUp();

	}

	//3ミスになったら画像表示
	if (isReach3Miss) {
		//読み込むddsを決定
		DecideDDS();
		DecidePos();
	}
	return true;
}

void Result::Update()
{
//	if (tf == true) {
//		return;
//	}
//	if (m_ui->GetIsTimeUp() && GetIsTimeUp() == false) {
//		//読み込むddsを決定
//		DecideDDS();
//		//スプライトを表示する位置を決定
//		DecidePos();
//		//タイムアップフラグを立てる
//		SetIsTimeUp();
//	
//	}
//
//	//3ミスになったら画像表示
//	if (isReach3Miss) {
//		//読み込むddsを決定
//		DecideDDS();
//		DecidePos();
//	}
//	tf = true;
}