#pragma once
class Counter : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	Vector3 CounterPos = { 900.0f, 0.0f, 400.0f };
	Vector3 CounterPos2 = CounterPos;
public:
	bool Start();
	void Update();
	void Delete();

	//bool Judge();

	void Render(RenderContext& rc) { model.Draw(rc); }

	int Delay = 0;
};

