#pragma once
class SpriteRender : public IGameObject
{
private:
	Sprite m_sprite;
	SpriteInitData m_spriteData;

	Vector3 m_Pos;
	Vector3 m_Scale = { 1.0f, 1.0f, 1.0f };
	Quaternion m_Rot = Quaternion::Identity;
	Vector2 m_Pivot = { 0.5f,0.5f };

public:
	SpriteRender() {};
	void Init(const char* name, int a, int b);
	void SetPosition(Vector3 pos);
	void SetScale(Vector3 scale);
	
	void Render(RenderContext& rc) { m_sprite.Draw(rc); }
};


