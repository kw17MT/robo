#pragma once
class SpriteRender;


class Rader : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_playerPos = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	Vector2 m_pivot = Vector2::Zero;

	int m_enemyNum = 0;
public:
	Rader() {}
	~Rader();
	bool Start();
	void Update();
	void SaveEnemyPos(int no, Vector3 enemyPos);
	void SetEnemyNum(int num) { m_enemyNum = num; }
	void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
private:
	Vector3 m_enemyPos[10];
	SpriteRender* m_spriteRender[10] = { nullptr };
	SpriteRender* m_playerIcon = nullptr;
};

