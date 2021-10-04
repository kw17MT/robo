#include "stdafx.h"
#include "PostEffect.h"

void PostEffect::Init(RenderTarget& defferedTarget,
	RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specDepthMap,
	RenderTarget& velocityMap)
{
	m_bloom.Init(defferedTarget);
	m_fxaa.Init(defferedTarget);
	m_motionBlur.InitSprite(defferedTarget, normalMap, specDepthMap, velocityMap);
	m_lensGhost.Init();
}

void PostEffect::Render(RenderContext& rc, RenderTarget& mainTarget, RenderTarget& defferedTarget)
{
	m_motionBlur.Render(rc, defferedTarget);
	m_bloom.Render(rc, defferedTarget);
	m_fxaa.Render(rc, mainTarget);
	m_lensGhost.Render(rc, mainTarget);
}