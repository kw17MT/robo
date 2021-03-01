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
	m_spriteData.m_ddsFilePath[2] = { "Assets/modelData/Green.dds" };


	m_spriteData.m_width = 5.0f;
	m_spriteData.m_height = 5.0f;

	m_sprite.Init(m_spriteData);

	return true;
}

void SpriteRender::Update()
{
	m_sprite.Draw(rc);
}