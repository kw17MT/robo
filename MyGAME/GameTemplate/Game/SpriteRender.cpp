#include "stdafx.h"
#include "SpriteRender.h"

void SpriteRender::Init(float w, float h)
{
	/*m_spriteData.m_ddsFilePath[2] = { "Assets/modelData/Green.dds" };


	m_spriteData.m_width = w;
	m_spriteData.m_height = h;

	m_sprite.Init(m_spriteData);*/
}

bool SpriteRender::Start()
{
	SpriteInitData spdata;
	spdata.m_ddsFilePath[0] = "Assets/image/menu.dds";
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	spdata.m_width = 128;
	spdata.m_height = 256;
	
	m_sprite.Init(m_spriteData);

	return true;
}

void SpriteRender::Update()
{
	
}