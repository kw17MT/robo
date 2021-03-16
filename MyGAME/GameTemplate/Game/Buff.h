#pragma once

class Buff : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//どちらに流れているバフアイテムか
	//１、左　２、右
	int BuffNo = 0;

public:
	//モデルデータ、影、初期位置設定。
	Buff();
	//アイテムの移動、Aボタンで消える処理。
	void Update();

	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	//どちら側に流れているかの設定。
	void SetBuffNo(int num) { BuffNo = num; }

	void Render(RenderContext& rc) { model.Draw(rc); }

	//Update()でインクリメントする。
	int time = 0;
	int BuffExist = 0;
};

