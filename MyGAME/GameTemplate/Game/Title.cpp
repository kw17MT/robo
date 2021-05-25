#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "Game.h"
//#include "OperationExplanation.h"

Title::~Title()
{
	DeleteGO(m_spriteRender);
}

bool Title::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/Title_512.dds", 1280, 720);

	return true;
}

void Title::Update()
{
	/*if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}*/

	if (m_fade == nullptr) {
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//フェード
			m_fade = NewGO<Fade>(0, "fade");
		}
		return;
	}
	//真っ暗になったら遷移
	if (m_fade->GetState() == enState_Wait) {
		//真っ暗になりきった段階でタイトルのスプライトを削除
		DeleteGO(m_spriteRender);
		//auto operationExplanation = NewGO<OperationExplanation>(0);
		//フェードインに移行
		m_fade->SetState(enState_In);
		//ゲームを生成
		NewGO<Game>(0, "game");
		//DeleteGO(this);
	}

	//フェード用画像の不等明度が0以下になったときにフェード画像とタイトル自身を削除
	if (m_fade->GetState() == enState_In && m_fade->GetAlpha() <= 0.0f) {
		DeleteGO(m_fade);
		DeleteGO(this);
	}

}