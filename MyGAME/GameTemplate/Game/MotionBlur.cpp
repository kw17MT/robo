#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& sceneMap, RenderTarget& normalMap, RenderTarget& specAndDepthMap, RenderTarget& velocityMap)
{
	m_motionBlurData.m_width = 1280;
	m_motionBlurData.m_height = 720;
	m_motionBlurData.m_textures[0] = &sceneMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[3] = &velocityMap.GetRenderTargetTexture();
	m_motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	m_motionBlurData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_motionBlurSprite.Init(m_motionBlurData);
}

void MotionBlur::Render(RenderContext& rc, RenderTarget& targetToApply)
{
	rc.WaitUntilToPossibleSetRenderTarget(targetToApply);
	rc.SetRenderTargetAndViewport(targetToApply);
	m_motionBlurSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(targetToApply);
}