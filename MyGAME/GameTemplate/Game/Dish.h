#pragma once
#include "PathMove.h"
class SkinModelRender;
class Guzai;
class PlayerGene;
class PathMove;
class DishSpeedManeger;
class CSoundSource;
class DishGene;
class GuzaiGene;

class Dish : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_guzaiPos = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	bool isHavingGuzai = false;							//自分（この皿）は具材を乗せているか。
	const int maxNum2Refill = 5;						//補充を開始する最大値		２はTo
	bool isCompletedFirstPop = false;					//全ての皿の上に具材を設置できたか
	float m_guzaiYPos = 1000.0f;						//具材の高さ。
	int m_guzaiTimer = 0;
	bool m_soundFlag = false;
	int m_guzaiNo;

	std::unique_ptr<PathMove> m_pathMove;		//移動処理を行うクラス。


	Light m_lig;
public:
	Dish() {}
	~Dish();

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetHavingState(bool state) { isHavingGuzai = state; }
	enCycleDirection GetCycleDirection() { return m_pathMove->GetCycleDirection(); }

	void Move();

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai = nullptr;
	PlayerGene* playerGene;
	DishSpeedManeger* m_speedManeger;
	CSoundSource* m_se;
	DishGene* m_dishGene;
	GuzaiGene* m_guzaiGene;
};

