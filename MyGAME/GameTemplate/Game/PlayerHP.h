#pragma once
#include "SpriteRender.h"
#include "FontRender.h"

enum EnPlayerDamageTypes
{
	enEnemyBullet,
	enEnemyMissile,
	enEnemyRaser
};

class PlayerHP : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -300.0f, 0.0f };
	Vector3 m_scale = Vector3::One;
	Vector2 m_pivot = { 1.0f, 0.0f };

	float m_playerHp = 100.0f;
	bool m_isPlayerAlive = true;
public:
	PlayerHP() {}
	~PlayerHP();
	bool Start();
	void Update();

	bool GetIsPlayerAlive() { return m_isPlayerAlive; }
	void ApplyDamage(EnPlayerDamageTypes enemyDamageType);
	void SetHPZero() { m_playerHp = 0.0f; }
private:
	SpriteRender* m_spriteRender = nullptr;
	FontRender* m_fontRender = nullptr;
};

