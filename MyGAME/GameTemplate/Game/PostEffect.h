#pragma once
#include "Bloom.h"
#include "FXAA.h"
#include "DepthInView.h"

class PostEffect
{
public:
	void Init(RenderTarget& mainRenderTarget);
	void Render(RenderContext& rc, RenderTarget& TargetToApply);
private:
	Bloom m_bloom;
	FXAA m_fxaa;
	DepthInView m_depthInView;
};