#pragma once
#include "Guzai.h"

class ModelRender : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	int playerNo = 0;
	int setPos = 0;

	//Vector3 pos = { 0.0f,0.0f,0.0f };
	//Vector3 scale = { 1.0f,1.0f,1.0f };
	//Quaternion rot;

public:
	ModelRender() {}
	void SetPlayerNo(int num) { playerNo = num; }
	/// <summary>
	/// デフォルトでUnityChanのパス
	/// </summary>
	bool Start();
	void Update();

	Vector3 GetPosition();

	void Render(RenderContext& rc)
	{
		model.Draw(rc);
	}

	void GiveData(const char* ModelPath, const char* ShaderPath);
	//プレイヤーが具材を持っているか。１なら持っている。
	int have = 0;
	//具材をなんか積み上げているか
	//今はキッチンがY0にあるため、便宜上初期値は１にする。
	int stack = 1;
};