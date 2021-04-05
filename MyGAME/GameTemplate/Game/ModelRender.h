#pragma once
class Guzai;
class FontRender;

class ModelRender : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//プレイヤーのパラメータ
	//座標
	Vector3 m_position = Vector3::Zero;
	//回転
	Quaternion m_rotation = Quaternion::Identity;
	//スケール
	Vector3 m_scale = Vector3::One;
	//角度
	float angle = 0.0f;
	//移動速度
	Vector3 moveSpeed = Vector3::Zero;

	//１で左、２で右
	int playerNo = 0;
	//int setPos = 0;

	//バフの効果が有効かどうか。
	bool Buff = false;
	//バフの有効時間。
	int BuffTime = 120;
	//今、具材をターゲットしているか。1個以上ターゲティングしないように。
	bool TargetingState = false;

public:
	ModelRender();
	~ModelRender();
	//モデル、当たり判定、具材No.を格納する配列を９で初期化
	//↑の処理をコンストラクタからStart関数に変更しました。
	bool Start();
	void SetPlayerNo(int num) { playerNo = num; }

	//移動処理。
	void Update();

	void SetBuffAffect(bool buff) { Buff = buff; }
	bool stateBuff() { return Buff; }
	
	//具材格納用配列を９で初期化
	void SetGuzaiNo9();

	//void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	//自身の持つ座標情報をキャラコンに渡して処理してもらうために変更しました。
	void SetPosition(Vector3 pos) { m_position.x = pos.x, m_position.y = pos.y, m_position.z = pos.z; }
	//キャラコンの座標 → プレイヤー自身の座標を取得するように変更
	Vector3 GetPosition();

	void Render(RenderContext& rc) { model.Draw(rc); }

	void GiveData(const char* ModelPath, const char* ShaderPath);

	//現在ターゲティングしているかどうか
	void SetTarget(bool target) { TargetingState = target; }
	//現在のターゲティング状態を返す。
	bool GetTargetState() { return TargetingState; }

	//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。
	int have = 0;
	//積み上げている具材の種類を格納していく。
	int GuzaiNo[10];

};