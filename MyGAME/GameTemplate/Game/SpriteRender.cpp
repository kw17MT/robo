#include "stdafx.h"
#include "SpriteRender.h"

void SpriteRender::Init(const char* name, int width, int height)
{
	SpriteInitData spdata;
	//画像のファイルパス名を設定
	spdata.m_ddsFilePath[0] = name;
	//シェーダーを設定
	spdata.m_fxFilePath = "Assets/shader/sprite.fx";
	//カラーフォーマットを設定
	spdata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//横幅を設定
	spdata.m_width = width;
	//高さを設定
	spdata.m_height = height;
	//アルファブレンディングモードを有効化
	spdata.m_alphaBlendMode = AlphaBlendMode_Trans; 

	m_sprite.Init(spdata);
}

void SpriteRender::Update()
{
	//位置、拡大率、中心軸を設定
	m_sprite.Update(m_pos, m_rot, m_scale, m_pivot);
}