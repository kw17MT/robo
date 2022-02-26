#pragma once
class SpriteRender;

class CrossIcon : public IGameObject
{
private:
	Vector3 m_scale = Vector3::One;
	Vector3 m_position = Vector3::Zero;
public:
	CrossIcon() {}
	~CrossIcon();
	bool Start();
	void Update();

	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetPosition(Vector3 pos) { m_position = pos; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

