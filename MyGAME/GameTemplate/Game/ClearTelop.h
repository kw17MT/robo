#pragma once
class SpriteRender;

class ClearTelop : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_surroundScale = Vector3::One;
	Vector3 m_buttomScale = Vector3::One;
	float m_angle = 0.0f;
	Quaternion m_rot = Quaternion::Identity;
public:
	ClearTelop() {}
	~ClearTelop();
	bool Start();
	void Update();
private:
	SpriteRender* m_sprite[3] = { nullptr };
};

