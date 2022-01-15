#pragma once
class FontRender;
class SpriteRender;

class ObjectiveEnemyNum : public IGameObject
{
private:
	float m_scale = 0.5f;
	int m_killedEnemyNum = 0;
	int m_objectiveNum = 0;

	enum EnFontTypes
	{
		enObjectiveNum,
		enKilledEnemyNum,
		enTypeNum
	};
public:
	ObjectiveEnemyNum() {};
	~ObjectiveEnemyNum();
	bool Start();
	void Update();

	void AddKilledEnemyNum() { m_killedEnemyNum++; }
	void SetObjectiveNum(int num) { m_objectiveNum = num; }
private:
	SpriteRender* m_sprite = nullptr;
	FontRender* m_font[enTypeNum] = { nullptr };
};

