#pragma once
#include "effect/Effect.h"

class Kitchen;
class SkinModelRender;
class Player;
class PlayerGene;
class GuzaiGene;
class GuzaiOkiba;
class TrashCan;
class CSoundSource;
class Effect;
class Meter;

class Guzai : public IGameObject
{
private:
	Vector3 m_position = { 0.0f,0.0f,-1000.0f };
	Vector3 m_scale = Vector3::One;
	const Vector3 m_dummyScale = { 1.3f,1.3f,1.3f };
	Quaternion m_rotation = Quaternion::Identity;
	float angle = 0.0f;
	
	//Update()でインクリメントする。
	int timer = 0;
	//Xボタン長押しで積み上げている具材を全部削除する。そのためのタイマー。
	int DeleteTimer = 0;
	//０．チーズ　１．エッグ　２．レタス　３．パテ　４．トマト
	//５．オニオン　６．ベーコン　※追加具材
	int TypeNo = 9;
	//具材からプレイヤーへの距離
	float guzai2Pl01 = 0.0f;
	float guzai2Pl02 = 0.0f;
	//キッチンからプレイヤーへの距離
	float kit2Pl01 = 0.0f;
	float kit2Pl02 = 0.0f;
	
	//ターゲティング関連
	//ターゲティングされた具材を設定するよう。
	bool targeted = false;
	//1度しかダミー具材を出さないようにするための変数。1ならダミーをすでに出した。
	bool isSetTargetDummy = false;
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
	//その具材は何であるかのメモ用、ターゲティングのオブジェクトを作成時に使用。
	const char* NowModelPath;

	//その具材はどちらのプレイヤーにつかまれたか
	int whichPlayerGet = 0;
	//どちらのプレイヤーにターゲットされたか
	int whichPlayerTargetMe = 0;
	//1~36
	int dishNumber = 0;
	//自身が具材置き場にセットされているか？
	bool m_guzaiOkibaSet = false;
	//自身がセットされた具材置き場の番号
	int m_setKitchenNum = 9;
	//自身が調理されているか？
	bool m_isCooked = false;
	//1Pの調理中の時間。
	int m_hold01 = 0;
	//2Pの調理中の時間。
	int m_hold02 = 0;
	//調理にかかる時間。
	int m_cookingTime = 60;
	//1Pの音を鳴らしたかのフラグ
	bool m_soundFlag01 = false;
	//2Pの音を鳴らしたかのフラグ
	bool m_soundFlag02 = false;

	//ゴミ箱に置いたのかどうか
	bool isSetOnTrashCan = false;

	bool canPutOnGuzaiOkiba = true;

	//自分の番号
	int m_myNo = 100;
public:	
	//乱数を用いて具材が生まれると同時にどの具材を流すか設定する。
	Guzai();
	~Guzai();
	bool Start();
	void Update();
	
	Vector3 GetPosition() { return m_position; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { GuzaiScale = scale; }

	//具材をキッチンに置いたときにオブジェクトを消すため、初めに設定しておいたTypeNoも消えてしまう。
	//つまり、レーン上に出したオブジェクトと新しくキッチン上に出すオブジェクトが違う。
	//プレイヤー側に保存しているTypeNoをキッチン側でNewGOするオブジェクトに与えてやる。
	//引数にプレイヤーに格納されている種類ナンバーを与える。
	void ChangeGuzai(int num);
	//キッチンに置いたときに違うモデルに差し替える用
	void ChangeModel(int& num);

	int GetTypeNo() { return TypeNo; }

	//具材の搬送
	void GrabNPut();
	//ターゲティングの対象選定とターゲットダミーを出す。
	//離れると別のターゲットを探す。
	void TargetingNPopDummy();
	//置かれた皿番号を記録する。
	void SetDishNumber(int num) { dishNumber = num; }

	//具材置き場に置く処理。
	void SetGuzaiOkiba();

	//具材置き場から取り出す処理。
	void GetGuzaiOkiba();

	//具材置き場で調理する処理。
	void Cooking();

	//ゴミ箱の上に具材をセット、後に消す。
	void SetOnTrashCan();

	//具材の回転処理。
	void Rotation();
	//どっちのプレイヤーに持たれているか。
	void SetWhichPlayerGet(int num) { whichPlayerGet = num; }

	//番号をセット
	void SetGuzaiNo(int number) { m_myNo = number; }

	//番号を入手
	int GetGuziNo() { return m_myNo; }

	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;
	//一度キッチンに置かれてから、また取られたとき
	bool returned = false;

	Player* pl01 = nullptr;
	Player* pl02 = nullptr;
	Kitchen* ki01 = nullptr;
	Kitchen* ki02 = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	PlayerGene* playerGene = nullptr;
	GuzaiGene* m_guzaiGene = nullptr;
	GuzaiOkiba* m_guzaiOkiba = nullptr;
	TrashCan* m_trashCan[2] = { nullptr, nullptr };
	CSoundSource* m_cookingSe = nullptr;

	SkinModelRender* targetDummyOnGuzai01 = nullptr;
	SkinModelRender* targetDummyOnGuzai02 = nullptr;

	Meter* m_meter = nullptr;
};

