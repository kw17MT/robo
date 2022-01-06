#include "stdafx.h"
#include "PostEffect.h"
#include "GameDirector.h"

void PostEffect::Init(RenderTarget& mainRenderTarget,
	RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specDepthMap,
	RenderTarget& velocityMap)
{
	m_depthInView.Init(mainRenderTarget, specDepthMap);
	m_bloom.Init(mainRenderTarget);
	m_fxaa.Init(mainRenderTarget);
	m_motionBlur.InitSprite(mainRenderTarget, normalMap, specDepthMap, velocityMap);
	m_lensGhost.Init();
}

void PostEffect::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget)
{
	if (GameDirector::GetInstance().GetGameScene() == enTitle)
	{
		m_depthInView.Render(rc, mainTarget);
	}
	else
	{
		m_motionBlur.Render(rc, mainSprite, mainTarget);
	}
	m_bloom.Render(rc, mainTarget);
	m_fxaa.Render(rc, mainTarget);
	m_lensGhost.Render(rc, mainTarget);
}