#pragma once

class ModelRender; //プレイヤー

class PopUp2D : public IGameObject
{
public:
	PopUp2D() {};
	~PopUp2D() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) { m_model.Draw(rc); }

private:
	Model m_model;
	
	//プレイヤー
	ModelRender* m_player01 = nullptr;
	ModelRender* m_player02 = nullptr;

	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	//プレイヤーのシチュエーション(追加予定)
	enum EnState {
		enNone,				//周囲に何もない
		enNearFood,			//具材が近くにある
		enNearBuffer,		//バフアイテムが近くにある
		enNearDebuffer,		//デバフアイテムが近くにある
		enNearKitchen,		//キッチンが近くにある
		enNearBurger,		//作成したバーガーが近くにある
		enNearCounter,		//カウンターが近くにある
		enDisposable,		//作りかけのバーガーか作ったバーガーがある
		enDerivable,		//バーガーを提供可能である
		enStateNum			//シチュエーション総数
	};

	EnState m_state = enNone;
};

