#pragma once
#include <vector>

class Guzai : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	//Skeleton m_skeleton;
	CharacterController m_charaCon;
	
	//Update()でインクリメントする。
	int timer = 0;
	//Xボタン長押しで積み上げている具材を全部削除する。そのためのタイマー。
	int DeleteTimer = 0;
	//どっち側に流れている具材か。
	int GuzaiNo = 0;
	//０．チーズ　１．エッグ　２．レタス　３．パテ　４．トマト
	int TypeNo = 9;
	//
	bool targeted = false;
	//
	Vector3 GuzaiScale = { 1.0f,1.0f,1.0f };

public:	
	//乱数を用いて具材が生まれると同時にどの具材を流すか設定する。
	Guzai();

	void Update();
	
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	void SetScale(Vector3 scale) { GuzaiScale = scale; }

	//具材をキッチンに置いたときにオブジェクトを消すため、初めに設定しておいたTypeNoも消えてしまう。
	//つまり、レーン上に出したオブジェクトと新しくキッチン上に出すオブジェクトが違う。
	//プレイヤー側に保存しているTypeNoをキッチン側でNewGOするオブジェクトに与えてやる。
	//引数にプレイヤーに格納されている種類ナンバーを与える。
	void ChangeGuzai(int num);
	//どちら側に流れている具材か設定する。
	void SetGuzaiNo(int num) { GuzaiNo = num; }

	int GetTypeNo() { return TypeNo; }

	void Render(RenderContext& rc) { model.Draw(rc); }

	
	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;
	
};

