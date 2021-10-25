#pragma once
class SpriteRender;

enum EnDamageTypes
{
	enBullet,
	enMissile,
	enRaser
};

class EnemyHP : public IGameObject
{
private:
	Vector3 m_enemyPos = Vector3::Zero;
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	int m_enemyHP = 100;
	bool m_isTargeted = false;
	bool m_isDead = false;
public:
	EnemyHP() {}
	~EnemyHP();
	bool Start();
	void Update();
	void IsEnemyTargeted(bool isTargeted) { m_isTargeted = isTargeted; }
	void SetEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }
	void ApplyDamage(EnDamageTypes damageType);
	bool IsDead() {return m_isDead;}
private:
	SpriteRender* m_spriteRender = nullptr;
};

