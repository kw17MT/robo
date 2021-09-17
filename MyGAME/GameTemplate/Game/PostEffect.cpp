#include "stdafx.h"
#include "PostEffect.h"

void PostEffect::Init(RenderTarget& mainRenderTarget,
	RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specDepthMap,
	RenderTarget& speedMap)
{
	m_bloom.Init(mainRenderTarget);
	m_fxaa.Init(mainRenderTarget);
	//m_depthInView.Init(mainRenderTarget);
	/*m_ssr.Init(mainRenderTarget,
		 depthRenderTarget,
		 normalRenderTarget,
		 metallicSmoothRenderTarget,
		 albedoRenderTarget);*/
	m_motionBlur.InitSprite(albedoMap, normalMap, specDepthMap, speedMap);
}

void PostEffect::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	//m_motionBlur.Draw(rc);
	m_bloom.Render(rc, TargetToApply);
	//m_depthInView.Render(rc, TargetToApply);
	m_fxaa.Render(rc, TargetToApply);
	//m_ssr.Render(rc, TargetToApply);
}