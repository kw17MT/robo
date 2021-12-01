#pragma once
class MissileTrail
{
public:
	void Init(RenderTarget& rt);

	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	RenderTarget m_trailTarget;
	SpriteInitData m_spriteData;
	Sprite m_trailSprite;
};

