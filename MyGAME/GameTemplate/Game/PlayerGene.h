#pragma once
class Player;

class PlayerGene : public IGameObject
{
private:
	Vector3 player01Pos = { 1000.0f,0.0f,0.0f };
	Vector3 player02Pos = { -1000.0f, 0.0f, 0.0f };
	Quaternion player01Rot;
	Quaternion player02Rot;

	int playerNum = 0;																//何人プレイヤーを生成したかの数
	bool isWorking = true;															//プレイヤー生成器が稼働中か　稼働中ならプレイヤー生成が終わっていない

	int noHavingDishCounter = 0;													//空の皿の数
	int refilledNum = 0;															//何個補充したか。Dishで使用。これがmaxNum2Refillと同値になると補充完了を意味し、noHavingDish、refilledNumを０で初期化する。
public:
	~PlayerGene() {};

	bool Start();
	void Update();

	//各プレイヤーの座標セッター
	void SetPlayer01Pos(Vector3 pos) {	player01Pos = pos; }
	void SetPlayer02Pos(Vector3 pos) {	player02Pos = pos; }

	//空の皿の計測関係
	int GetNoHavingDishCounter() { return noHavingDishCounter; }					//空の皿のゲッタ―
	void AddNoHavingDishCounter() { noHavingDishCounter++; }						//空の皿の数を1つ足す
	void ResetNohavingDishCounter() { noHavingDishCounter = 0; }					//補充完了時に空の皿を０に戻す
	//補充した皿関係	
	int GetRefilledNum() { return refilledNum; }									//補充完了した皿の数
	void AddRefilledNum() { refilledNum++; }										//補充完了した皿の数を一つ足す
	void ResetRefilledNum() { refilledNum = 0; }									//補充完了時に補充した皿を０に戻す

	//プレイヤー生成器電源状態,true = 動いてる, false = 止まってる。
	bool GetPlayerGeneState() { return isWorking; }

	Player* player[2];
};

