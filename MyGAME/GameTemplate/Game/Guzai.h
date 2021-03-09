#pragma once
#include <vector>

class Guzai : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

public:	
	bool Start();
	void Update();
	
	Vector3 GetPosition();
	void SetPosition(Vector3 pos);

	void Render(RenderContext& rc) { model.Draw(rc); }

	//Update()でインクリメントする。
	int time = 0;
	//１ならば持たれている。
	int state = 0;
	//１ならばもうキッチンに置かれている。
	int put = 0;
	//何段積んでるか
	int stack = 0;
	//Xボタン長押しで積み上げている具材を全部削除する。そのためのタイマー。
	int DeleteTimer = 0;
	//具材の種類番号
	//ToDo...変更可能にする。
	int TypeNo = 1;
};

