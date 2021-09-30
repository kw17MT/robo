#include "stdafx.h"
#include "PlanarReflection.h"

void PlanarReflection::InitSprite(RenderTarget& sceneTarget)
{
	SpriteInitData spriteData;
	spriteData.m_width = 1280;
	spriteData.m_height = 720;
	spriteData.m_textures[0] = &sceneTarget.GetRenderTargetTexture();
	spriteData.m_fxFilePath = "Assets/shader/PlanarReflection.fx";
	spriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	spriteData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_sprite.Init(spriteData);
}

void PlanarReflection::Render(RenderContext& rc)
{

}