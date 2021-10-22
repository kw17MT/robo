#pragma once
class EnemyGenerator;
class Player;
class SkyCube;
class Ground;
class UI;
class Sun;

class Game : public IGameObject
{
public:
	Game() {};
	~Game();
	bool Start();
	void Update();
private:
	Player* m_player = nullptr;
	EnemyGenerator* m_enemyGenerator = nullptr;
	SkyCube* m_sky = nullptr;
	Ground* m_ground = nullptr;
	UI* m_ui = nullptr;
	Sun* m_sun = nullptr;
};

