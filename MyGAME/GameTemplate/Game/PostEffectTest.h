#pragma once
class PostEffectTest : public IGameObject
{
private:
	ModelInitData modelData;
	Model model;
	CharacterController m_charaCon;

	Vector3 m_pos = { 0.0f,0.0f,0.0f };
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_scale = { 1.0f,1.0f,1.0f };

public:
	PostEffectTest() {}
	~PostEffectTest() {}

	bool Start();
	void Update();

	void Render(RenderContext& rc) { model.Draw(rc); }
};

