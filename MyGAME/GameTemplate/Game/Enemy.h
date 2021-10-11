#pragma once
class SkinModelRender;
class DisplayDistance;
class Player;

class Enemy : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
public:
	bool Start();
	void Update();
private:
	SkinModelRender* m_skinModelRender = nullptr;
	DisplayDistance* m_displayDistance = nullptr;
	Player* m_player = nullptr;
};

