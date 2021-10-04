#pragma once
class SkinModelRender;

class Player : public IGameObject
{
private:
	Vector3 m_prevPosition = Vector3::Zero;
	Vector3 m_currentPosition = Vector3::Zero;
	
public:
	Player() {};
	~Player() {};
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender;
};

