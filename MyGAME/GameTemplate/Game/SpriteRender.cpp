#include "stdafx.h"
#include "SpriteRender.h"
#include "FixedUI.h"

void SpriteRender::Init(const char* name, int a, int b)
{
	SpriteInitData spdata;
	spdata.m_ddsFilePath[0] = name;
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	spdata.m_width = a;
	spdata.m_height = b;

	m_sprite.Init(spdata);
}
