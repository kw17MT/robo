#pragma once
class PostEffectTest : public IGameObject
{
private:
	ModelInitData modelData;
	Model model;
	CharacterController m_charaCon;
	Skeleton m_skeleton;

	Vector3 m_pos = { 0.0f,0.0f,0.0f };
	Quaternion m_rot = Quaternion::Identity;
	Vector3 m_scale = { 1.0f,1.0f,1.0f };

	RootSignature root;
	RenderTarget mainRenderTarget;
	RenderTarget luminanceRenderTarget;
	SpriteInitData luminanceSpriteData;
	Sprite luminanceSprite;
	//çÏê¨ó\íË
	//GaussianBlur gaussianBlur;

public:
	PostEffectTest() {}
	~PostEffectTest() {}

	bool Start();
	void Update();

	void InitRootSig();
	void InitMainRenderTarget();
	void InitLuminanceRenderTarget();

	void Render(RenderContext& rc) { model.Draw(rc); }
};

