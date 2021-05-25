#include "stdafx.h"
#include "Result.h"
#include "SpriteRender.h"
#include "FixedUI.h"
#include "GameDirector.h"

Result::~Result()
{
	//DeleteGO(this);
	DeleteGO(m_spriteRender);
}

void Result::DecideDDS()
{
	//spriteNumの値に応じて初期化に使用するDDSを振り分ける
	switch (spriteNum) {
	case 0: //引き分け
		//m_spriteRender = NewGO<SpriteRender>(1);
		m_spriteRender->Init("Assets/Image/draw.dds", wideth*0.75f, height*0.75f);
		break;
	case 1: //勝利
		//m_spriteRender = NewGO<SpriteRender>(1);
		m_spriteRender->Init("Assets/Image/win.dds", wideth*0.75f, height*0.75f);
		break;
	case 2: //敗北
		//m_spriteRender = NewGO<SpriteRender>(1);
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
	m_spriteRender = NewGO<SpriteRender>(1);
	

	//中央、左、右の画像表示位置用変数を設定
	center.Set(0.0f,0.0f,0.0f);
	left.Set(0.0f + 450.0f, 0.0f,0.0f);
	right.Set(0.0f - 450.0f, 0.0f,0.0f);

	//UIを検索
	if (m_ui == nullptr) {
		m_ui = FindGO<FixedUI>("ui");
	}

	if (m_ui->GetIsTimeUp() && isTimeUp == false) {
		//読み込むddsを決定
		DecideDDS();
		//スプライトを表示する位置を決定
		DecidePos();
		//タイムアップフラグを立てる
		isTimeUp = true;
	}

	//3ミスになったら画像表示
	if (isReach3Miss) {
		//読み込むddsを決定
		DecideDDS();
		DecidePos();
	}

	m_spriteRender->SetPivot({0.5f,0.5f});
	m_spriteRender->SetScale({ 1.0f * m_scaleRate,1.0f * m_scaleRate,1.0f });
	m_spriteRender->SetColor({ 1.0f,1.0f,1.0f,m_alpha });

	return true;
}

void Result::Update()
{
	m_scaleRate -= 0.08f;
	if (m_scaleRate <= 1.0f) {
		m_scaleRate = 1.0f;
	}

	m_alpha += 0.03f;
	if (m_alpha >= 1.0f) {
		m_alpha = 1.0f;
	}



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

	m_spriteRender->SetColor({ 1.0f,1.0f,1.0f,m_alpha });
	m_spriteRender->SetScale({ 1.0f * m_scaleRate,1.0f * m_scaleRate,1.0f });

}