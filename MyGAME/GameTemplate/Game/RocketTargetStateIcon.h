#pragma once
class SpriteRender;

class RocketTargetStateIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;
	Vector3 m_targetedEnemyPos = Vector3::Zero;
	Vector3 m_scale = Vector3::Zero;
	bool m_isFirstExpand = false;
public:
	RocketTargetStateIcon() {}
	~RocketTargetStateIcon();
	bool Start();
	void Update();
	void SetTargetedEnemyPos(Vector3 enemyPos) { m_targetedEnemyPos = enemyPos; }
	void SetScale(Vector3 scale) { m_scale = scale; }
	void SetFirstExpandScale(bool state) { m_isFirstExpand = state; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

