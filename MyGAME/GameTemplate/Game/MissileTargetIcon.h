#pragma once
class SpriteRender;
class Enemy;

class MissileTargetIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_targetedEnemyPos = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	bool m_isFirstExpand = false;
public:
	MissileTargetIcon() {}
	~MissileTargetIcon();
	bool Start();
	void Update();
	void SetTargetedEnemyPos(Vector3 enemyPos) { m_targetedEnemyPos = enemyPos; }
	void SetTargetedEnemy(Enemy* enemy) { m_enemy = enemy; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetFirstExpandScale(bool state) { m_isFirstExpand = state; }
private:
	SpriteRender* m_spriteRender = nullptr;
	Enemy* m_enemy = nullptr;					//アイコンを表示する敵のインスタンス
};

