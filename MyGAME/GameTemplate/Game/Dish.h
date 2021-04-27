#pragma once
class SkinModelRender;

class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	bool isHavingGuzai = true;
	//自分はWayPointの何番目の皿なのか
	int myNumber = 0;

public:
	Dish() {}
	~Dish() {}

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }
	void SetMyNumber(int num) { myNumber = num; }

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
};

