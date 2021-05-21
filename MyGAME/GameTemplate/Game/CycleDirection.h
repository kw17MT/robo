#pragma once
class SpriteRender;

enum EnDirection {
	Forward,		//回転スプライト正転
	Reverse,		//回転スプライト逆転
	FixedForward,	//固定表示スプライト正転
	FixedReverse,	//固定表示スプライト逆転
};

enum EnSide {
	Left,			//左
	Right,			//右
	Up				//画面上部
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
	void ChangeFixedSpriteForward();
	void ChangeFixedSpriteReverse();

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

