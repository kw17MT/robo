#pragma once
#include "Bloom.h"
#include "FXAA.h"
#include "DepthInView.h"
#include "SSR.h"
#include "MotionBlur.h"

class PostEffect
{
public:
	void Init(RenderTarget& mainRenderTarget, 
		RenderTarget& albedoMap,
		RenderTarget& normalMap,
		RenderTarget& specDepthMap,
		RenderTarget& speedMap
		 );

	void Render(RenderContext& rc, RenderTarget& TargetToApply);
private:
	Bloom m_bloom;
	FXAA m_fxaa;
	DepthInView m_depthInView;
	SSR m_ssr;
	MotionBlur m_motionBlur;
};