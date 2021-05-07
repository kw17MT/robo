#pragma once
#include "PathMove.h"
class SkinModelRender;
class Guzai;
class PlayerGene;
class PathMove;


class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_guzaiPos = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	bool isHavingGuzai = false;
	//自分はWayPointの何番目の皿なのか
	int myNumber = 0;
	
	//補充を開始する最大値		２はTo
	const int maxNum2Refill = 5;
	//全ての皿の上に具材を設置できたか
	bool isCompletedFirstPop = false;
	//PlayerGeneに具材を持っていない皿の数があるが、わざわざここに持ってくる。
	//空の皿上に具材を出した後、noHavingDishを初期化したいが、一度行ってしまうと、1個しか補充できなくなるため必要。
	int totalNoHavingDish = 0;
	

	std::unique_ptr<PathMove> m_pathMove;		//移動処理を行うクラス。

public:
	Dish() {}
	~Dish() {}

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }
	void SetMyNumber(int num) { myNumber = num; }

	void Move();

	bool Start();
	void Update();

	

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai;
	PlayerGene* playerGene;
};

