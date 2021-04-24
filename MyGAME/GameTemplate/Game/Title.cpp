#include "stdafx.h"
#include "Title.h"
#include "Game.h"

Title::~Title()
{
	DeleteGO(m_spriteRender);
}

bool Title::Start()
{

	m_spriteRender = NewGO<SpriteRender>(0);

	m_spriteRender->Init("Assets/Image/title.dds", 1280, 720);

	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		NewGO<Game>(0, "game");
		DeleteGO(this);
	}
}