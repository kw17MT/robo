#pragma once
class MotionBlur
{
public:
	void InitSprite(RenderTarget& defferedTarget, RenderTarget& normalTarget, RenderTarget& specAndDepthTarget, RenderTarget& velocityTarget);

	void Render(RenderContext& rc, Sprite& mainSprite , RenderTarget& targetToApply);

private:
	RenderTarget m_motionBlurTarget;
	Sprite m_motionBlurSprite;
	GaussianBlur m_gaussian[4];
	Sprite m_finalGaussian;
};

