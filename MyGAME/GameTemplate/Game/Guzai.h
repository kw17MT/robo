#pragma once
#include <vector>
class ModelRender;
class Kitchen;

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
	//具材からプレイヤーへの距離
	float guzai2Pl = 0;
	//キッチンからプレイヤーへの距離
	float kit2Pl = 0;
	
	//ターゲティング関連
	//ターゲティングされた具材を設定するよう。
	bool targeted = false;
	//1度しかダミー具材を出さないようにするための変数。1ならダミーをすでに出した。
	int SetTargetDummy = 0;
	//一度ターゲッティングしたものをある程度見つめるための時間。
	//HOLDTIMEが待ち時間。DECREMENTTIMEが徐々に減らしていく役割がある。
	const int holdTime = 10;
	int decrementTime = holdTime;
	//ターゲットする最短距離。
	const int TargetRangeNear = 100;
	//ターゲッティングを切り始める距離。（実際はディレイがあるため消えるのはもうちょっと先になる）
	const int TargetRangeFar = 110;
	//枠線強調のために使用
	Vector3 GuzaiScale = { 1.0f,1.0f,1.0f };
	

public:	
	//乱数を用いて具材が生まれると同時にどの具材を流すか設定する。
	Guzai();
	bool Start();
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

	//具材の移動。
	void Move();
	//具材の搬送
	void GrabNPut();
	//ターゲティングの対象選定とターゲットダミーを出す。
	//離れると別のターゲットを探す。
	void TargetingNPopDummy();



	void Render(RenderContext& rc) { model.Draw(rc); }

	
	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;

	ModelRender* pl01;
	ModelRender* pl02;
	Kitchen* ki01;
	Kitchen* ki02;
};

