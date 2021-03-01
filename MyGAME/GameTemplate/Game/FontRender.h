#pragma once

class FontRender : public IGameObject
{
private:
	int time = 120;
	int score01 = 0;
	int score02 = 0;
	int Counter = 0;

	Vector2 posTime = { -100,350 };
	Vector2 posItem01 = { -600,350 };
	Vector2 posItem02 = { 350, 350 };
	Vector2 posScore01 = { -600,-300 };
	Vector2 posScore02 = { 350, -300 };

	RenderContext renderContext = g_graphicsEngine->GetRenderContext();
public:
	bool Start();
	void Update();

	int GetTime() { return time; }
	
	wchar_t String2Wcahr(int num);
};


