#pragma once
class SpriteRender;

enum EnDirection {
	Forward,
	Reverse,
};

enum EnSide {
	Left,
	Right
};

class CycleDirection : public IGameObject
{
public:
	CycleDirection() {};
	~CycleDirection();
	bool Start();
	void Update();
	void ChangeSpriteForward();
	void ChangeSpriteReverse();

	//セッター
	void SetDirection(const EnDirection& enDirection) { m_direction = enDirection; }
	void SetSide(const EnSide& enSide) { m_side = enSide; }
	//ゲッター
	EnDirection GetDirection() { return m_direction; }
	EnSide GetSide() { return m_side; }

private:
	SpriteRender* m_sprite = nullptr;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = { 0.5f,0.5f,1.0f };
	EnDirection m_direction = Forward;
	EnSide m_side = Left;

	float angle = 0.0f;
	float alpha = 0.0f;
};

