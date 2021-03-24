#pragma once
class Guzai;

class ModelRender : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//１で左、２で右
	int playerNo = 0;
	//int setPos = 0;

	//バフの効果が有効かどうか。
	bool Buff = false;
	//バフの有効時間。
	int BuffTime = 120;
	//今、具材をターゲットしているか。
	bool SetTarget = false;

public:
	//モデル、当たり判定、具材No.を格納する配列を９で初期化
	ModelRender();
	void SetPlayerNo(int num) { playerNo = num; }

	//移動処理。
	void Update();

	void SetBuffAffect(bool buff) { Buff = buff; }
	bool stateBuff() { return Buff; }
	//具材格納用配列を９で初期化
	void SetGuzaiNo9();

	void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	Vector3 GetPosition();

	void Render(RenderContext& rc) { model.Draw(rc); }

	void GiveData(const char* ModelPath, const char* ShaderPath);

	//ターゲットを現在しているかどうか
	//void SetTarget(bool target) { SetTarget = target; }

	//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。
	int have = 0;
	//積み上げている具材の種類を格納していく。
	int GuzaiNo[10];
};