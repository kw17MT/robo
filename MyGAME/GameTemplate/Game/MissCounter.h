#pragma once
class SpriteRender;
class Level2D;
#include "Level2D.h"

class MissCounter : public IGameObject
{
private:
	int pl1MissCount = 0;
	int pl2MissCount = 0;

	bool shouldChangeFilePath = false;
public:
	bool Start();
	void Update();

	void AddPl1MissCount() { pl1MissCount++; }
	void AddPl2MissCount() { pl2MissCount++; }

	void ChangeMarkState(bool state) { shouldChangeFilePath = state; }

	Level2D m_level2D;		//ƒŒƒxƒ‹2DB
	SpriteRender* m_spriteRender[6];
};

