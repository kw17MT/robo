#pragma once
class Counter : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	//トマトだけで構成されたバーガー
	int TomatoOnly[1] = { 4 };
	int burger01[3] = { 3,1,2 };
	int burger02[4] = { 3,4,4,2 };
	int burger03[4] = { 2,4,0,3 };

	//カウンター番号　１が左、２が右
	int CounterNo = 0;

public:
	Counter();
	bool Start();
	void Update();
	void Delete();
	void SetPosition(Vector3 pos) { m_charaCon.SetPosition(pos); }
	void SetCounterNo(int num) { CounterNo = num; }

	bool Judge();

	void Render(RenderContext& rc) { model.Draw(rc); }

	int Delay = 0;
};

