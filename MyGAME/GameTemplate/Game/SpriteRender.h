#pragma once
class SpriteRender : public IGameObject
{
public:
	SpriteRender() {};
	void Init(float w, float h);
	bool Start();
	void Update();

private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;

	void Render(RenderContext& rc) { m_sprite.Draw(rc); }
};


