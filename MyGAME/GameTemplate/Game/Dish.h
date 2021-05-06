#pragma once
#include "PathMove.h"
class SkinModelRender;
class Guzai;
class PlayerGene;
class PathMove;
class DishSpeedManeger;

class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_guzaiPos = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	bool isHavingGuzai = false;							//自分（この皿）は具材を乗せているか。
	const int maxNum2Refill = 5;						//補充を開始する最大値		２はTo
	bool isCompletedFirstPop = false;					//全ての皿の上に具材を設置できたか
	

	std::unique_ptr<PathMove> m_pathMove;		//移動処理を行うクラス。

public:
	Dish() {}
	~Dish() {}

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }

	void Move();

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai;
	PlayerGene* playerGene;
	DishSpeedManeger* m_speedManeger;
};

