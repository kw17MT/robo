#pragma onece

class ModelRender_new : public IGameObject{
private:
	Model m_model;
	ModelInitData m_modelInitData;
	Skeleton m_skeleton;
	//CharacterController m_characterController;


	Vector3 m_position;
	Quaternion m_rotation = Quaternion::Identity;
	Vector3 m_scale = Vector3::One;

public:
	ModelRender_new() {};
	~ModelRender_new();
	//モデルの初期化
	void Init(const char* filePath,EnModelUpAxis modelUpAxis);
	//各種セッター
	void SetPosition(Vector3& pos);
	void SetRotation(Quaternion& rot);
	void SetScale(Vector3& scale);

	void Render(RenderContext& rc) { m_model.Draw(rc); }

};