#pragma once
class LensGhost
{
public:
	void Init();
	
	void Render(RenderContext& rc, RenderTarget& target);

private:
	RenderTarget m_ghostTarget;
	Sprite m_lensGhost;
	SpriteInitData lensGhostData;
};

