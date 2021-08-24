#include "stdafx.h"
#include "PostEffect.h"

void PostEffect::Init(RenderTarget& mainRenderTarget,
	RenderTarget& depthRenderTarget,
	RenderTarget& normalRenderTarget,
	RenderTarget& metallicSmoothRenderTarget,
	RenderTarget& albedoRenderTarget )
{
	m_bloom.Init(mainRenderTarget);
	m_fxaa.Init(mainRenderTarget);
	//m_depthInView.Init(mainRenderTarget);
	/*m_ssr.Init(mainRenderTarget,
		 depthRenderTarget,
		 normalRenderTarget,
		 metallicSmoothRenderTarget,
		 albedoRenderTarget);*/
}

void PostEffect::Init(RenderTarget& mainRenderTarget)
{
	m_bloom.Init(mainRenderTarget);
	m_fxaa.Init(mainRenderTarget);
}

void PostEffect::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	m_bloom.Render(rc, TargetToApply);
	//m_depthInView.Render(rc, TargetToApply);
	m_fxaa.Render(rc, TargetToApply);
	//m_ssr.Render(rc, TargetToApply);
}