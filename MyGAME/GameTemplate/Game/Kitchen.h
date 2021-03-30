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

	//どっち側のキッチンか
	int KitchenNo = 0;
	//具材がキッチンに置かれると具材CPP側でインクリメント
	int stack = 0;
	//次、何番目の要素になるかの番号
	int nextStackNum = 0;
	//キッチンにおける最大数。
	const int MaxStack = 5;
	//とりあえず、10個までしか積み上げられない。
	//バグ防止のため、5個積み上げたらコンベアから具材を持っていけない。
	Guzai* StackedGuzai[5];
	//スタックが完了したかどうか。これによって1フレームのずらしを起こしたい。
	bool isCompletedStack = false;

public:
	//キッチンのモデルなどの設定。
	Kitchen();
	void Update();
	//キッチン番号を設定する。
	void SetKitchenNo(int num) { KitchenNo = num;}
	//キッチンの座標を設定する。
	void SetKitchenPos(Vector3 pos) { m_charaCon.SetPosition(pos); }
	//キッチンの座標を取得
	Vector3 GetKitchenPos() { return m_charaCon.GetPosition(); }
	//具材をキッチンの上に発生させる。
	void Stack(int num);
	//キッチン上の具材を全消去
	void Delete();
	int DeleteTimer = 0;
	//privateの変数スタックをインクリメント
	void PlusStack() { stack++; }
	//数えていたStack変数を０に初期化。
	void SetStack0() { stack = 0; }
	//プレイヤー側に具材の種類を格納する際に使用する。
	int GetStackNum() { return stack; }
	//具材をハンバーガーに変換する。
	//消す時遅延を起こしてやらないとエラーを吐くためそれ対策
	void BornBurger();
	int Delay = 60;
	//具材を消す時に、プレイヤーに格納されている種類No.を９で初期化する。
	//この関数は具材を消す前に使うこと。
	void ClearNo();

	void Render(RenderContext& rc) { model.Draw(rc); }

	Burger* bur;
};

