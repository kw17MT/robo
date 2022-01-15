#pragma once

enum EnGameScene
{
	enTitle,
	enExit,
	enLaunchingPlayer,
	enInGame,
	enGameClear,
	enToTitle
};

class GameDirector
{
private:
	static GameDirector* instance;
	EnGameScene enGameScene = enTitle;
public:
	static void CreateInstance()
	{
		instance = new GameDirector;
	}
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}
	static void DeleteInstance()
	{
		delete instance;
	}

	EnGameScene GetGameScene()
	{
		return enGameScene;
	}

	void SetGameScene(EnGameScene scene)
	{
		enGameScene = scene;
	}
};

