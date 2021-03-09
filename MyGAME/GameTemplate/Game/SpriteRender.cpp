#include "stdafx.h"
#include "SpriteRender.h"

void SpriteRender::Init(const char* name, int a, int b)
{
	SpriteInitData spdata;
	spdata.m_ddsFilePath[0] = name;
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	spdata.m_width = a;
	spdata.m_height = b;

	m_sprite.Init(spdata);
}

void SpriteRender::SetPosition(Vector3 pos)
{
	m_Pos = pos;
	m_sprite.Update(m_Pos, m_Rot, m_Scale, m_Pivot);
}

void SpriteRender::SetScale(Vector3 scale)
{
	m_Scale = scale;
	m_sprite.Update(m_Pos, m_Rot, m_Scale, m_Pivot);
}

