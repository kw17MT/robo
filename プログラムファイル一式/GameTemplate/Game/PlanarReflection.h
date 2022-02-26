#pragma once
class PlanarReflection
{
public:
	void InitSprite(RenderTarget& sceneTarget);
	void Render(RenderContext& rc);
private:
	RenderTarget m_refTarget;
	Sprite m_sprite;
};