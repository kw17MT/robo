#pragma once
class SpriteRender
{
public:
	//SpriteRender() {};
	void Init(float w, float h);
	bool Start();
	void Update();

private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;
	RenderContext& rc;
};


