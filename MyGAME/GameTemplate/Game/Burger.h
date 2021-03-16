#pragma once

class Burger : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//どちらに流れているバーガーか
	int BurgerNo = 0;
	
	Vector3 pos;

public:
	//モデルデータ、影の設定。
	Burger();
	//バーガーの位置をキッチンの上にして、具材が消える同時にバーガーを出現させる。
	bool Start();
	//バーガーをDeleteGO。
	void Delete();
	//プレイヤーがバーガーを持つ処理を行う。
	void GrabBurger();
	//バーガーを消した際、カウンターに積んでいた具材の種類を初期化。
	//プレイヤーに保存していた、ハンバーガーを構成する具材を９で初期化。	
	void ClearNo();

	void Update();
	
	void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	//バーガーはどちら側のか...１、左　２、右
	void SetBurgerNo(int num) { BurgerNo = num; }

	void Render(RenderContext& rc) { model.Draw(rc); }

	//ボタン長押しでバーガーを消すようにするため。
	int DeleteTimer = 0;
	//１の時に上のDelete(),ClearNo()を
	int burgerExist = 0;
	int putOnKitchen = 0;
	
	
	Vector3 CounterPos = { 900.0f, 100.0f, -400.0f };
};

