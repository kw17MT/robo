#pragma once

class Level;
class Guzai;
class Player;
class PlayerGene;

class SkinModelRender;
class Kitchen;


class GuzaiOkiba : public IGameObject
{
private:
	Vector3 m_kitchenPos[8];					//具材置き場の座標
	int m_kitchenNo;							//具材置き場の番号
	Vector3 m_playerPos[2];						//プレイヤーの座標
	float m_distance[8];						//プレイヤーと具材置き場の距離
	int m_guzaiNo;								//具材の番号(種類)
	bool m_kitchenSet[8] = { false };			//具材置き場にセット可能か？ trueでセット可能 falseでセット不可能
	bool m_guzaiSet[8] = { false };				//具材置き場のセット状況 trueで置かれている falseで置かれていない
	bool m_targeted01 = false;					//1Pがターゲット中か？
	bool m_targeted02 = false;					//2Pがターゲット中か？
	Vector3 m_targetPos01 = Vector3::Zero;		//1Pがアクセス可能な具材置き場の座標
	Vector3 m_targetPos02 = Vector3::Zero;		//2Pがアクセス可能な具材置き場の座標
	float m_targetDistance01;					//1Pがアクセス可能な具材置き場の座標と1Pの距離
	float m_targetDistance02;					//2Pがアクセス可能な具材置き場の座標と2Pの距離
	int m_targetNo01;							//1Pがアクセス可能な具材置き場の番号
	int m_targetNo02;							//2Pがアクセス可能な具材置き場の番号

public:
	GuzaiOkiba();
	~GuzaiOkiba();
	bool Start();
	void Update();

	//受け取った2つのベクトルの距離を返す関数
	float TargetDistance(Vector3 v1, Vector3 v2);

	//プレイヤーと具材置き場の距離を測る関数
	void PlayerDistance();

	//プレイヤーがどの具材置き場にアクセス出来るか決める関数
	void Targeted();

	//具材置き場のセット可能状況を入手 引数に具材置き場の番号を入力(0～7)
	bool FindKitchenSet(int num) { return m_kitchenSet[num]; }

	//具材置き場の状態を入手 引数に具材置き場の番号を入力(0～7) trueで具材が置かれている falseで置かれていない
	bool FindGuzaiSet(int num) { return m_guzaiSet[num]; }

	//具材置き場の状態を変更したことを伝える関数 第一引数引数に具材置き場の番号を入力(0～7) 第二引数は具材セット完了でtrue 具材取り出しでfalse
	void GuzaiSet(int num, bool tf) { m_guzaiSet[num] = tf; }

	//具材置き場の座標を入手 引数に具材置き場の番号を入力(0～7)
	Vector3 GetKitchenPos(int num) { return m_kitchenPos[num]; }


	Player* pl01 = nullptr;
	Player* pl02 = nullptr;
	Guzai* guzai = nullptr;
	Kitchen* stock[8];
	SkinModelRender* m_guzaiRender01;		//1Pがターゲット中にアクセス可能な具材置き場に出すモデル
	SkinModelRender* m_guzaiRender02;		//2Pがターゲット中にアクセス可能な具材置き場に出すモデル
};

