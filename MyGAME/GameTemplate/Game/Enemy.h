#pragma once
class SkinModelRender;
class DisplayDistance;
class Player;
class EnemyStateIcon;
class EnemyBrain;
class MachinGun;

#include "EnemyHP.h"
#include "EnemyMove.h"

class Enemy : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Quaternion m_rot = Quaternion::Identity;
	int m_distance = 0;
public:
	Enemy() {};
	~Enemy();
	bool Start();
	void Update();
	void SetPosition(Vector3 pos) { m_position = pos; }

	Vector3 GetPosition() { return m_position; }
	void TakenDamage(EnDamageTypes damageType);
private:
	SkinModelRender* m_skinModelRender = nullptr;
	DisplayDistance* m_displayDistance = nullptr;
	Player* m_player = nullptr;
	EnemyStateIcon* m_enemyStateIcon = nullptr;

	EnemyBrain* m_enemyBrain = nullptr;
	EnemyMove m_enemyMove;
	EnemyHP* m_enemyHP = nullptr;
	//MachinGun* m_machinGun = nullptr;
};

