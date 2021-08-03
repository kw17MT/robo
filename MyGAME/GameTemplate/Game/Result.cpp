#include "stdafx.h"
#include "Result.h"
#include "SpriteRender.h"
#include "FixedUI.h"
#include "GameDirector.h"

namespace
{
	const int DRAW = 0;
	const int WIN = 1;
	const int LOSE = 2;

	const float AJUST_SPRITE_SCALE = 0.75f;
	const float AJUST_SPRITE_HALF = 0.5f;
	const float SHRINK_RATE = 0.08f;
	const float ALPHA_INCREASE_RATE = 0.03f;
	const float FIXED_SCALE_ONE_Z = 1.0f;
}

Result::~Result()
{
	DeleteGO(m_spriteRender);
}

void Result::DecideDDS()
{
	//spriteNumの値に応じて初期化に使用するDDSを振り分ける
	switch (m_spriteNum) {
	case DRAW:
		m_spriteRender->Init("Assets/Image/draw.dds", static_cast<int>(m_wideth * AJUST_SPRITE_SCALE), static_cast<int>(m_height * AJUST_SPRITE_SCALE));
		break;
	case WIN:
		m_spriteRender->Init("Assets/Image/win.dds", static_cast<int>(m_wideth * AJUST_SPRITE_SCALE), static_cast<int>(m_height * AJUST_SPRITE_SCALE));
		break;
	case LOSE: 
		m_spriteRender->Init("Assets/Image/lose.dds", static_cast<int>(m_wideth * AJUST_SPRITE_HALF), static_cast<int>(m_height * AJUST_SPRITE_HALF));
		break;
	default:
		break;
	}
}

void Result::DecidePos()
{
	//positionNumの値に応じて表示位置を振り分ける
	switch (m_positionNum) {
	case DRAW: //引き分け : 画面中央くらい
		m_spriteRender->SetPosition(m_center);
		break;
	case WIN: //左側
		m_spriteRender->SetPosition(m_left);
		break;
	case LOSE://右側
		m_spriteRender->SetPosition(m_right);
		break;
	default:
		break;
	}
}

bool Result::Start()
{
	m_spriteRender = NewGO<SpriteRender>(1);

	//UIを検索
	if (m_ui == nullptr) {
		m_ui = FindGO<FixedUI>("ui");
	}

	if (m_ui->GetIsTimeUp()) {
		//読み込むddsを決定
		DecideDDS();
		//スプライトを表示する位置を決定
		DecidePos();
	}

	//3ミスになったら画像表示
	if (m_isReach3Miss) {
		//読み込むddsを決定
		DecideDDS();
		//出す位置を決定する。
		DecidePos();
	}

	m_spriteRender->SetPivot(m_pivot);
	m_spriteRender->SetScale({m_scaleRate, m_scaleRate, FIXED_SCALE_ONE_Z });
	m_spriteRender->SetColor(m_color);

	return true;
}

void Result::Update()
{
	//出現時、縮小していく。
	m_scaleRate -= SHRINK_RATE;
	if (m_scaleRate <= 1.0f) {
		m_scaleRate = 1.0f;
	}
	//透明度をおおきくしていく
	m_alpha += ALPHA_INCREASE_RATE;
	if (m_alpha >= 1.0f) {
		m_alpha = 1.0f;
	}
	//透明度の更新
	m_color.w = m_alpha;
	//色の更新
	m_spriteRender->SetColor(m_color);
	m_spriteRender->SetScale({m_scaleRate,m_scaleRate,FIXED_SCALE_ONE_Z });
}