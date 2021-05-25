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
	static const int guzaiNum = 36;

	bool isCompletedSetGuzai = false;

	int emptyDishNumber[6];
	int emptyDishCounter = 0;
	const int LostNumber = 5;
	int m_guzaiNum = 0;					//具材の出ている数
	bool m_guzaiFlag[99] = { false };	//具材が出ているかどうかのフラグ。

	Vector3 m_setPos = Vector3::Zero;
public:
	GuzaiGene() {};
	~GuzaiGene();
	//生成器の座標設定。
	bool Start();
	//具材を生成する。
	//ランダム変数を用いてバフを出すか否かを決めている。
	void Update();

	bool GetInitialActionState() { return isCompletedSetGuzai; }

	//具材が存在しているか？(引数に番号を入力)
	bool GetGuzaiFlag(int number) { return m_guzaiFlag[number]; }

	//具材の存在状態を変更。(引数に番号を入力)
	void SetGuzaiFlag(int number, bool tf) { m_guzaiFlag[number] = tf; }

	SkinModelRender* m_skinModelRender;
	Guzai* m_guzai[99] = {nullptr};
	DishGene* m_dishGene;
	//Buff* m_buff;
};
