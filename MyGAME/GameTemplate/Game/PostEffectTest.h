#pragma once
class RenderContext;

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

	//以下GameObjectManagerで初期化、実行したもの。
	//RootSignature root;
	////通常のレンダリング対象
	//RenderTarget mainRenderTarget;
	////輝度抽出用のレンダリング対象
	//RenderTarget luminanceRenderTarget;
	////輝度依存のぼかしを適応した画像用
	//SpriteInitData luminanceSpriteData;
	//Sprite luminanceSprite;

	////作成予定
	////ガウシアンブラーをかけるためのもの。
	//GaussianBlur gaussianBlur;

	//SpriteInitData finalSpriteData;
	//Sprite finalSprite;

	//SpriteInitData copyToFrameBufferSpriteData;
	//Sprite copyToFrameBufferSprite;

	//RenderContext& renderContext = g_graphicsEngine->GetRenderContext();

public:
	PostEffectTest() {}
	~PostEffectTest() {}

	bool Start();
	void Update();

	void Render(RenderContext& rc) { model.Draw(rc); }
};

