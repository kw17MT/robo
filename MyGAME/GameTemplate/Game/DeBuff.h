#pragma once

class PathMove;

class DeBuff : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//どちらに流れているバフアイテムか
	//１、左　２、右
	int BuffNo = 0;

	//移動処理を行うクラス。
	std::unique_ptr<PathMove> m_pathMove;

public:
	//モデルデータ、影、初期位置設定。
	DeBuff();

	bool Start();

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

	bool m_isDeBuffLane = false;
};


