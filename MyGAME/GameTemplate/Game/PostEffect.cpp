#include "stdafx.h"
#include "PostEffect.h"

void PostEffect::Init(RenderTarget& mainRenderTarget)
{
	m_bloom.Init(mainRenderTarget);
	m_fxaa.Init(mainRenderTarget);
	m_depthInView.Init(mainRenderTarget);
}

void PostEffect::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	m_bloom.Render(rc, TargetToApply);
	//m_depthInView.Render(rc, TargetToApply);
	m_fxaa.Render(rc, TargetToApply);
}