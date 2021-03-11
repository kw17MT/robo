#pragma once

class Buff : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//どちらに流れているバフアイテムか
	int BuffNo = 0;

public:
	Buff();

	bool Start();
	void Update();

	Vector3 GetPosition();
	void SetPosition(Vector3 pos);
	void SetBuffNo(int num) { BuffNo = num; }

	void Render(RenderContext& rc)
	{
		model.Draw(rc);
	}

	//Update()でインクリメントする。
	int time = 0;
	//何番目に作成された箱か
	int exist = 0;
	//Start()で待つ時間を決めている。
	int wait = 0;
	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;

};

