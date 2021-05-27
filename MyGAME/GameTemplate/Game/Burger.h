
#pragma once

class SkinModelRender;
class TrashCan;
class Player;
class CSoundSourse;

class Effect;
#include "effect/Effect.h"

class CSoundSourse;


class Burger : public IGameObject
{
private:
	Vector3 m_position;
	Vector3 m_scale;
	//どちらに流れているバーガーか
	int BurgerNo = 0;
	bool isSetOnTrashCan = false;

	int decrementTime = 20;
	
	Vector3 pos;


public:
	//モデルデータ、影の設定。
	Burger();
	~Burger();
	//バーガーの位置をキッチンの上にして、具材が消える同時にバーガーを出現させる。
	bool Start();
	//バーガーをDeleteGO。
	void Delete();
	//プレイヤーがバーガーを持つ処理を行う。
	void GrabBurger();
	//バーガーを消した際、カウンターに積んでいた具材の種類を初期化。
	//プレイヤーに保存していた、ハンバーガーを構成する具材を９で初期化。	
	void ClearNo();
	//カウンター側でバーガーのモデルを消すために使う
	void ClearModel() { DeleteGO(this); }

	void Update();
	
	void SetPosition(Vector3 pos) { m_position = pos; }
	//バーガーはどちら側のか...１、左　２、右
	void SetBurgerNo(int num) { BurgerNo = num; }
	//ゴミ箱にセットして消す
	void SetOnTrashCan();

	//ボタン長押しでバーガーを消すようにするため。
	int DeleteTimer = 0;
	//１の時に上のDelete(),ClearNo()を
	int burgerExist = 0;
	int putOnKitchen = 0;
	
	Vector3 CounterPos = { 900.0f, 100.0f, -400.0f };

	Effect m_effect;
	SkinModelRender* m_skinModelRender = nullptr;
	Player* m_player[2] = { nullptr };
	TrashCan* m_trashCan[2] = { nullptr };
};