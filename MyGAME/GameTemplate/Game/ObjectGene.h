#pragma once
class Guzai;
class Buff;

class ObjectGene : public IGameObject
{
private:
	//Generator用のモデル
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//ジェネレーターの番号。1が右、2が左
	int GeneNo = 0;

public:
	//生成器の座標設定。
	ObjectGene();
	//具材やバフアイテムを生成する。
	//ランダム変数を用いてバフを出すか否かを決めている。
	void Update();
	void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	Vector3 GetPosition() { return m_charaCon.GetPosition(); }
	//生成器が左か右かどちらにあるか設定しておく。　１、左　２、右
	void SetGeneNo(int num) { GeneNo = num; }
	
	//具材を流すタイミングを決めるもの。
	int timer = 0;
	//バフが存在していると１、ないと０。　０でないとバフアイテムは流れない。
	int Buffnum = 0;

	Guzai* m_guzai;
	Buff* m_buff;
};
