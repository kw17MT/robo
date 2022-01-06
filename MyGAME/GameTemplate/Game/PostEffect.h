#pragma once
#include "Bloom.h"
#include "FXAA.h"
#include "DepthInView.h"
#include "SSR.h"
#include "MotionBlur.h"
#include "LensGhost.h"

class PostEffect
{
public:
	void Init(RenderTarget& defferedTarget, 
		RenderTarget& albedoMap,
		RenderTarget& normalMap,
		RenderTarget& specDepthMap,
		RenderTarget& speedMap
		 );

	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	DepthInView m_depthInView;
	Bloom m_bloom;
	FXAA m_fxaa;
	MotionBlur m_motionBlur;
	LensGhost m_lensGhost;
};