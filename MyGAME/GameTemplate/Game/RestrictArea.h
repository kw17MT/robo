#pragma once
class SpriteRender;

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
public:
	RestrictArea() {}
	~RestrictArea();
	bool Start();
	void Update();

	void JudgeInArea(Vector3 playerPos);
	EnAreaTypes GetAreaType() { return m_areaType; }
private:
	SpriteRender* m_spriteRender = nullptr;
};

