#pragma once
#include "FontRender.h"

class DisplayDistance : public IGameObject
{
private:
	int Distance = 0;
	Vector2 m_position = Vector2::Zero;
	Vector3 m_enemyPos = Vector3::Zero;
	int m_distance = 0;
public:
	DisplayDistance() {};
	~DisplayDistance();
	bool Start();
	void Update();

	const int CalcDistance(Vector3 enemyPos, Vector3 playerPos);

	void SetEnemyPos(Vector3& pos)
	{
		m_enemyPos = pos;
	}

	void SetPosition(Vector2 pos)
	{
		m_position = pos;
	}
private:
	FontRender* m_fontRender = nullptr;
};

