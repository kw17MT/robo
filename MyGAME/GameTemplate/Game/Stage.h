#pragma once
class Stage
{
private:
	Model model;
	ModelInitData modeldata;
	CharacterController m_charaCon;

	Vector3 m_Pos;
	Vector3 m_Scale;
	Quaternion m_Rot;

public:
	bool Start();
	void Update();

	Vector3 GetPosition();

	void Render(RenderContext& rc)
	{
		model.Draw(rc);
	}
};

