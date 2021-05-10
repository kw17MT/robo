#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
	m_spriteRender = NewGO<SpriteRender>(20);
	m_spriteRender->Init("Assets/Image/title.dds", 1280, 720);
	//透明
	m_spriteRender->SetColor(0, 0, 0, m_alpha);
	//フェードアウト
	m_state = enState_Out;
}

Fade::~Fade()
{
	DeleteGO(m_spriteRender);
}

void Fade::Update()
{
	switch (m_state)
	{
	case enState_In:
		m_alpha -= 0.01f;
		//インしたら削除
		if (m_alpha <= 0.0f) {
			DeleteGO(this);
		}
		break;
	case enState_Out:
		m_alpha += 0.01f;
		//真っ暗で待機
		if (m_alpha >= 1.0f) {
			m_state = enState_Wait;
		}
		break;
	}
	//カラーは黒くしてる。
	m_spriteRender->SetColor(0, 0, 0, m_alpha);
}