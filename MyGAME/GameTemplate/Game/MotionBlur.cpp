#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& albedoMap, RenderTarget& normalMap, RenderTarget& specAndDepthMap, RenderTarget& speedMap)
{
	m_motionBlurData.m_width = 1280;
	m_motionBlurData.m_height = 720;
	m_motionBlurData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[3] = &speedMap.GetRenderTargetTexture();
	m_motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	m_motionBlurData.m_alphaBlendMode = AlphaBlendMode_Add;
	//m_motionBlurData.m_expandConstantBuffer = (void*)&m_vpMatrix;
	//m_motionBlurData.m_expandConstantBufferSize = sizeof(m_vpMatrix);
	m_motionBlurSprite.Init(m_motionBlurData);
}