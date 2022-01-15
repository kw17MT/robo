#include "stdafx.h"
#include "EnemySpawnPoint.h"

Vector3 EnemySpawnPoint::DecideSpawnPoint()
{
	int dir = rand() % enDirectionNum;

	return m_spawnPosition[dir];
}