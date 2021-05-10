#include "stdafx.h"
#include "Title.h"
#include "Fade.h"
#include "OperationExplanation.h"

Title::~Title()
{
	DeleteGO(m_spriteRender);
}

bool Title::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/title.dds", 1280, 720);


	/*Vector4 color = { 1.0f,1.0f,1.0f,0.5f };
	m_spriteRender->SetColor(color);*/

	return true;
}

void Title::Update()
{
	if (m_fade == nullptr) {
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//フェード
			m_fade = NewGO<Fade>(0, "fade");
		}
		return;
	}
	//真っ暗になったら遷移
	if (m_fade->GetState() == m_fade->enState_Wait) {
		auto operationExplanation = NewGO<OperationExplanation>(0);
		m_fade->SetState(m_fade->enState_In);
		DeleteGO(this);
	}
}