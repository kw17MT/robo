#include "stdafx.h"
#include "MapChip2D.h"
#include "CaslFile.h"

void MapChip2D::Init(CaslData* caslData)
{
	SpriteInitData data;
	data.m_width = caslData->width;
	data.m_height = caslData->height;

	data.m_ddsFilePath[0] = caslData->ddsFilePath.get();
	data.m_fxFilePath = "Assets/shader/sprite.fx";
	m_sprite.Init(data);

	m_position.x = caslData->position.x;
	m_position.y = caslData->position.y;

	m_scale.x = caslData->scale.x;
	m_scale.y = caslData->scale.y;

}