#pragma once
class MotionBlur
{
public:
	void InitSprite(RenderTarget& albedoMap, RenderTarget& normalMap, RenderTarget& specAndDepthMap, RenderTarget& velocityMap);

	void Render(RenderContext& rc, RenderTarget& targetToApply);

private:
	SpriteInitData m_motionBlurData;
	Sprite m_motionBlurSprite;
};

