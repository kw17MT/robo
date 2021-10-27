#pragma once
class MotionBlur
{
public:
	void InitSprite(RenderTarget& defferedTarget, RenderTarget& normalTarget, RenderTarget& specAndDepthTarget, RenderTarget& velocityTarget);

	void Render(RenderContext& rc, Sprite& mainSprite , RenderTarget& targetToApply);

private:
	RenderTarget m_motionBlurTarget;
	SpriteInitData m_motionBlurData;
	Sprite m_motionBlurSprite;
};

