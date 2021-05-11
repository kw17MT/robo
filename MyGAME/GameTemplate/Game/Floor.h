#pragma once
class SkinModelRender;

class Floor : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
public:
	void SetPosition(Vector3 pos) { m_position = pos; }

	bool Start();
	void Update();

	SkinModelRender* m_skinModelRender;
};

