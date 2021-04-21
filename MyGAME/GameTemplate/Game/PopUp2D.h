#pragma once

//class ModelRender;		//プレイヤー
class Guzai;			//具材
class Kitchen;			//キッチン
class Counter;			//カウンター

//自身はどのプレイヤーのものか
enum EnSelf {
	enPlayer01,			//プレイヤー1
	enPlayer02,			//プレイヤー2
	enPlayerNum			//要素数
};

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

class PopUp2D : public IGameObject
{
public:
	PopUp2D() {};
	~PopUp2D() {};
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) { m_model.Draw(rc); }
	
	//セッター、ゲッター
	void SetPosition(const Vector3& pos) { m_position = pos; }
	Vector3 GetPosition() { return m_position; };
	void SetRotation(const Quaternion& rot) { m_rotation = rot; }
	Quaternion GetRotation() { return m_rotation; }
	void SetScale(const Vector3& scale) { m_scale = scale; }
	Vector3 GetScale() { return m_scale; }
	
	EnSelf GetEnSelf() { return m_self; }
	void SetEnSelf(const EnSelf& self) { m_self = self; }
	EnState GetEnState() { return m_state; }
	void SetEnState(const EnState& state) { m_state = state; }

	void Move();
	void Rotation();
	void ChangeState();
	void ChangeModel();
	
private:
	//プレイヤー
	//ModelRender* m_player = nullptr;
	Guzai* m_guzai = nullptr;
	std::vector<Guzai*> vecGuzai;
	std::size_t vecGuzaiSize = 0;
	const int MAXSIZE = 10;

	Kitchen* m_kitchen = nullptr;
	Counter* m_counter = nullptr;

	//モデル
	Model m_model;
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;
	EnSelf m_self = enPlayer01;
	EnState m_state = enNone;

	//角度制御用に使うかも
	const float PI = 3.1415f;

};

