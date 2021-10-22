#pragma once
class SkinModelRender;

class Sun : public IGameObject
{
private:
	Vector3 m_position = { 0.0f,1000000.0f,0.0f };
	Vector3 m_scale = { 600.0f,600.0f,600.0f };
public:
	Sun() {};
	~Sun();
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;
};

