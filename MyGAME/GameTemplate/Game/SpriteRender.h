#pragma once
class SpriteRender
{
public:
	void Init(const char* path, float w, float h);
	bool Start();
	void Update();

private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;
	Texture m_texture;
};


