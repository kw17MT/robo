#pragma once
class SkinModelRender;

class Ground : public IGameObject
{
private:
	/*Model model;
	ModelInitData modeldata;
	Skeleton m_skeleton;
	CharacterController m_charaCon;*/

	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	RenderTarget shadowMap;
	Camera lightCamera;

public:
	Ground() {};
	~Ground() {};

	bool Start();
	void Update();

	//void Render(RenderContext& rc) { model.Draw(rc); }

	SkinModelRender* m_skinModelRender;
};