#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "Game.h"
//#include "OperationExplanation.h"

Title::~Title()
{
	//DeleteGO(m_spriteRender);
}

bool Title::Start()
{
	/*m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/Title_512.dds", 1280, 720);*/

	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}

	//if (m_fade == nullptr) {
	//	if (g_pad[0]->IsTrigger(enButtonA))
	//	{
	//		//フェード
	//		m_fade = NewGO<Fade>(0, "fade");
	//	}
	//	return;
	//}
	////真っ暗になったら遷移
	//if (m_fade->GetState() == m_fade->enState_Wait) {
	//	//auto operationExplanation = NewGO<OperationExplanation>(0);
	//	m_fade->SetState(m_fade->enState_In);
	//	NewGO<Game>(0, "game");
	//	DeleteGO(this);
	//}
}