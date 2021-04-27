#pragma once
class Guzai;
//class Buff;
class DishGene;
class SkinModelRender;

#include <vector>

class GuzaiGene : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	const Vector3 m_scale = Vector3::Zero;
	static const int guzaiNum = 37;

	bool isCompletedInitialAction = false;
public:
	GuzaiGene() {};
	~GuzaiGene() {};
	//生成器の座標設定。
	bool Start();
	//具材を生成する。
	//ランダム変数を用いてバフを出すか否かを決めている。
	void Update();

	bool GetInitialActionState() { return isCompletedInitialAction; }
	//具材を流すタイミングを決めるもの。
	//int timer = 0;
	//バフが存在していると１、ないと０。　０でないとバフアイテムは流れない。
	//int Buffnum = 0;
	//int DeBuffnum = 0;

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai[guzaiNum];
	DishGene* m_dishGene;
	//Buff* m_buff;
};
