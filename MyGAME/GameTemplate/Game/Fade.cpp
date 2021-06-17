#include "stdafx.h"
#include "Fade.h"

namespace
{
	Vector4 BLACK = { 0.0f,0.0f,0.0f,0.0f };

	const int WIDTH = 1280;
	const int HEIGHT = 720;
	const float FADE_SPEED = 0.01f;
	const float COLOR_ZERO = 0.0f;
}


Fade::Fade()
{
	m_spriteRender = NewGO<SpriteRender>(20);
	m_spriteRender->Init("Assets/Image/title.dds", WIDTH, HEIGHT);
	//透明
	BLACK.w = m_alpha;
	m_spriteRender->SetColor(BLACK);
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
		m_alpha -= FADE_SPEED;
		//インしたら削除
		if (m_alpha <= 0.0f) {
			DeleteGO(this);
		}
		break;
	case enState_Out:
		m_alpha += FADE_SPEED;
		//真っ暗で待機
		if (m_alpha >= 1.0f) {
			m_state = enState_Wait;
		}
		break;
	}

	BLACK.w = m_alpha;
	//カラーは黒くしてる。
	m_spriteRender->SetColor(BLACK);
}