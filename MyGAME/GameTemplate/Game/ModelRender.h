#pragma once
class Guzai;

class ModelRender : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	int playerNo = 0;
	int setPos = 0;

	bool Buff = false;
	int BuffTime = 120;

public:
	ModelRender();
	void SetPlayerNo(int num) { playerNo = num; }

	bool Start();
	void Update();

	void SetBuffAffect(bool buff) { Buff = buff; }
	bool stateBuff() { return Buff; }
	void SetGuzaiNo9();

	Vector3 GetPosition();

	void Render(RenderContext& rc) { model.Draw(rc); }

	void GiveData(const char* ModelPath, const char* ShaderPath);
	//プレイヤーが具材を持っているか。１なら具材を持っている。２ならハンバーガーを持っている。
	int have = 0;
	//積み上げている具材の種類を格納していく。
	int GuzaiNo[10];
};