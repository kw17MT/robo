#pragma once
class Guzai;
class Burger;

class Kitchen : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;


	Vector3 KitchenPos = { 900.0f, 0.0f, 0.0f };

	//具材がキッチンに置かれると具材CPP側でインクリメント
	int stack = 0;
	//次、何番目の要素になるかの番号
	int nextStackNum = 0;
	//とりあえず、10個までしか積み上げられない。
	//バグ防止のため、5個積み上げたらコンベアから具材を持っていけない。
	Guzai* StackedGuzai[5];

public:
	//モデルデータの読み込み
	bool Start();
	void Update();
	//具材をキッチンの上に発生させる。
	void Stack(int num);
	//キッチン上の具材を全消去
	void Delete();
	int DeleteTimer = 0;
	//privateの変数スタックをインクリメント
	void PlusStack() { stack++; }
	//具材5個をハンバーガーに変換する。
	//消す時遅延を起こしてやらないとエラーを吐くためそれ対策
	void BornBurger();
	int Delay = 60;
	//具材を消す時に、プレイヤーに格納されている種類No.を０で初期化する。
	//この関数は具材を消す前に使うこと。
	void ClearNo();

	void Render(RenderContext& rc) { model.Draw(rc); }

	Burger* bur;
};

