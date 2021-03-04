#pragma once
class Kitchen : public IGameObject
{
private:
	Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

public:
	bool Start();
	void Update();

	void Render(RenderContext& rc)
	{
		model.Draw(rc);
	}
};

