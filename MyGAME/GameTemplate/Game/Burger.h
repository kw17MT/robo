#pragma once

class Burger : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

public:
	bool Start();
	void Update();
	void SetPosition(Vector3 pos);

	void GrabBurger();

	void Render(RenderContext& rc) { model.Draw(rc); }

	void Delete();
	int DeleteTimer = 0;
	int burgerExist = 0;

	Vector3 pos = { 900.0f,100.0f, 0.0f };
};

