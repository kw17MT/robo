#pragma once
class MotionBlur
{
public:
	void InitSprite(RenderTarget& albedoMap, RenderTarget& normalMap, RenderTarget& specAndDepthMap, RenderTarget& speedMap);

	void Draw(RenderContext& rc)
	{
		m_motionBlurSprite.Draw(rc);
	}
private:
	SpriteInitData m_motionBlurData;
	Sprite m_motionBlurSprite;
};

