#pragma once
class SpriteRender;
class Effect;

enum EnAreaTypes
{
	enSafeArea,
	enSemiDangerArea,
	enDanger
};

class RestrictArea : public IGameObject
{
private:
	Vector3 m_scale = { 0.0f,0.0f,0.0f };
	EnAreaTypes m_areaType = enSafeArea;
	float alpha = 0.0f;
	bool m_addAlpha = true;
	float m_timeFromDeath = 0.0f;
	float m_areaRestrictTimer = 0.0f;
	float m_distance = 0.0f;
public:
	RestrictArea() {}
	~RestrictArea();
	bool Start();
	void Update();

	void JudgeInArea(Vector3 playerPos);
	EnAreaTypes GetAreaType() { return m_areaType; }
	float GetMonochromeRateDependOnDistance();
private:
	SpriteRender* m_spriteRender = nullptr;
	Effect* m_effect = nullptr;
	Effect* m = nullptr;
};

