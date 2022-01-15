#pragma once

/**
 * @brief 敵をスポーンさせる方角（プレイヤーが初めに向いている方向を北とする）
*/
enum EnSpawnPointDirection
{
	enNorth,
	enNorthWest,
	enWest,
	enSouthWest,
	enSouth,
	enSouthEast,
	enEast,
	enNorthEast,
	enDirectionNum
};

class EnemySpawnPoint
{
private:
	const Vector3 m_spawnPosition[enDirectionNum]
	{
		{0.0f,0.0f,-50000.0f},
		{35355.0f ,0.0f, -35355.0f},
		{50000.0f,0.0f,0.0f},
		{35355.0f ,0.0f, 35355.0f},
		{0.0f,0.0f,50000.0f},
		{-35355.0f ,0.0f, 35355.0f},
		{-50000.0f,0.0f,0.0f},
		{-35355.0f ,0.0f, -35355.0f},
	};
public:
	Vector3 DecideSpawnPoint();
	Vector3 GetNorthPosition() { return m_spawnPosition[enNorth]; }
private:
};

