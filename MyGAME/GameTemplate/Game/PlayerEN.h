#pragma once
class SpriteRender;

class PlayerEN : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -350.0f, 0.0f };
	Vector3 m_scale = Vector3::One;
	Vector2 m_pivot = { 1.0f, 0.0f };

	float m_playerEN = 100.0f;
	bool m_isPlayerEnRemain = true;
public:
	PlayerEN() {}
	~PlayerEN();
	bool Start();
	void Update();

	bool GetIsPlayerENRemain() { return m_isPlayerEnRemain; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

