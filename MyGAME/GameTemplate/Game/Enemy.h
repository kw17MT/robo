#pragma once
class SkinModelRender;
class DisplayDistance;
class Player;
class EnemyStateIcon;

class Enemy : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	int m_distance = 0;
public:
	bool Start();
	void Update();
	void SetPosition(Vector3 pos)
	{
		m_position = pos;
	}
private:
	SkinModelRender* m_skinModelRender = nullptr;
	DisplayDistance* m_displayDistance = nullptr;
	Player* m_player = nullptr;
	EnemyStateIcon* m_enemyStateIcon = nullptr;

	//CharacterController m_charaCon;
};

