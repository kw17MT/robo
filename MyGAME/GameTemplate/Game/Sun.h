#pragma once
class SkinModelRender;

class Sun : public IGameObject
{
private:
	Vector3 m_position = { 0.0f,0.0f,-500000.0f };
	Vector3 m_scale = { 300.0f,300.0f,300.0f };
public:
	Sun() {};
	~Sun();
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};

