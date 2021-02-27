#include "stdafx.h"
#include "SpriteRender.h"

void SpriteRender::Init(const char* path, float w, float h)
{
	//m_texture.InitFromDDSFile = path;


	m_spriteData.m_width = w;
	m_spriteData.m_height = h;

	m_sprite.Init(m_spriteData);
}

bool SpriteRender::Start()
{
	return true;
}

void SpriteRender::Update()
{
	
}