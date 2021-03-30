#include "stdafx.h"
#include "SpriteRender.h"
#include "FixedUI.h"

void SpriteRender::Init(const char* name, int width, int height)
{
	SpriteInitData spdata;
	spdata.m_ddsFilePath[0] = name;
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	spdata.m_width = width;
	spdata.m_height = height;

	m_sprite.Init(spdata);
}
