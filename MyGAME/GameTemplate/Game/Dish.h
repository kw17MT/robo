#pragma once
class SkinModelRender;

class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	bool isHavingGuzai = false;

public:
	Dish() {}
	~Dish() {}

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
};

