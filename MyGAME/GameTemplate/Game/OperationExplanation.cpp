#include "stdafx.h"
#include "OperationExplanation.h"
#include "Fade.h"
#include "Game.h"

OperationExplanation::OperationExplanation()
{
	//初期ステート
	m_currentState = enState_Idle;
	//ファイルパス
	m_spriteData[0].fliePath = "Assets/Image/title.dds";
	m_spriteData[1].fliePath = "Assets/Image/title.dds";
	m_spriteData[2].fliePath = "Assets/Image/title.dds";
	m_spriteData[3].fliePath = "Assets/Image/title.dds";
	m_spriteData[4].fliePath = "Assets/Image/title.dds";
	m_spriteData[5].fliePath = "Assets/Image/title.dds";
	//スプライトレンダー初期化
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init(m_spriteData[0].fliePath, 1280, 720);

	m_fade = FindGO<Fade>("fade");
}

OperationExplanation::~OperationExplanation()
{
	DeleteGO(m_spriteRender);
}

void OperationExplanation::Update()
{
	switch (m_currentState)
	{
	case enState_Change:
		//切り替え
		a -= 80;
		b -= 60;
		m_spriteRender->Init(m_spriteData[m_currentPage].fliePath, a, b);
		m_currentPage++;
		m_currentState = enState_Idle;
		break;
	case enState_End:
		//遷移
		if (m_fade == nullptr) {
			m_fade = NewGO<Fade>(0, "fade");
		}
		if (m_fade->GetState() == m_fade->enState_Wait) {
			NewGO<Game>(0, "game");
			m_fade->SetState(m_fade->enState_In);
			DeleteGO(this);
		}
		break;
	default:
		//入力
		if (m_fade == nullptr) {
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				if (m_currentPage < 6) {
					m_currentState = enState_Change;
				}
				else {
					m_currentState = enState_End;
				}
			}
			return;
		}
		m_fade = FindGO<Fade>("fade");
		break;
	}
}