#pragma once
class SkinModelRender;
class Meter : public IGameObject
{
	

private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = { 1.4f,1.0f,1.0f };
	Quaternion m_rot = Quaternion::Identity;

public:
	Meter();
	~Meter();
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position.x = pos.x, m_position.y = pos.y, m_position.z = pos.z; }
	void SetScale(Vector3 scale) { m_scale.x = scale.x, m_scale.y = scale.y, m_scale.z = scale.z; }
	Vector3 GetScale() { return m_scale; }

	Light m_lig;
	SkinModelRender* m_skinModelRender = nullptr;
};

