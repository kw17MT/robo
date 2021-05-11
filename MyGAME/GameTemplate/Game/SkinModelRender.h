#pragma once
class AnimationClip;

class SkinModelRender :public IGameObject
{
private:
	Model m_model;
	ModelInitData m_modelInitData;
	Skeleton m_skeleton;
	CharacterController m_charaCon;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	Quaternion m_rot = Quaternion::Identity;

public:
	SkinModelRender();
	~SkinModelRender();

	bool Start();
	void Update();

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetRotation(Quaternion rot) { m_rot = rot; }
	void SetNewModel() { m_model.Init(m_modelInitData); }

	Vector3 GetPosition() { return m_position; }
	Vector3 GetScale() { return m_scale; }


	void Init(const char* modelFilePath, EnModelUpAxis UpAxis, Vector3 pos);
	//モデルのファイルパスと初期位置を設定する。
	void Init(const char* filePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* pLig = nullptr);
	//モデルのファイルパスのみを変更するときに使用する。
	void ChangeModel(const char* newModelFilePath);
	//シェーダーのファイルパスと
	void InitShader(const char* filePath, const char* entryPointPath, const char* skinEntryPointPath, DXGI_FORMAT colorBuffer);
	//主にフロントカリングに変えるときに使用する
	void SetFrontCulling(const char* psEntryPoint) { m_modelInitData.m_psEntryPointFunc = psEntryPoint; }
	//影響を受けるライトの種類の変更。
	void InitLight(Light& light);
	//影を落とす背景用の初期化関数
	void InitBackGroundLight(Texture texture, Matrix lightcamera, Matrix lightcamerasize);
	//アニメーションを設定する。
	void InitAnimation(AnimationClip* animation, int animationNum);
	//引数２は補完時間
	void PlayAnimation(int animNo, float interpolateTime = 0.0f);

	void Render(RenderContext& rc) { m_model.Draw(rc); }

	Animation m_animation;
	AnimationClip* m_animationClip;
};

