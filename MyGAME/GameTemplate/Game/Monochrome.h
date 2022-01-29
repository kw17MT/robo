#pragma once

class Monochrome
{
private:
	float m_monochromeRate = 0.0f;
public:
	void Init();
	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	RenderTarget m_monochromedTarget;
	Sprite m_monochromedSprite;
};

