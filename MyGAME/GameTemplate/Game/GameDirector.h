#pragma once

enum EnScene
{
	enNonScene = -1,	//タイトル
	enGameCountDown,	//ゲーム開始前のカウントダウン。
	enGamePlay,			//ゲームプレイ中。
	enPause,			//ポーズ中
	enResult,			//リザルト中
	enGameEnd,			//ゲーム終了
	enSceneNumber
};

/// <summary>
/// ゲームのシーンを知ってるクラス。
/// </summary>
class GameDirector
{
private:
	GameDirector() {}
	~GameDirector() {}
public:
	//GameDirectorのインスタンスを取得。
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}

	//ゲームシーンを設定。
	void SetGameScene(EnScene enScene)
	{
		m_enScene = enScene;
	}
	
	//今がどんなシーンが取得。
	EnScene GetGameScene() const
	{
		return m_enScene;
	}
	
 	//カウントダウン中？
	bool GetIsGameCountDown() const
	{
		return m_enScene == enGameCountDown;
	}

	//ゲームプレイ中？
	bool GetIsGamePlay() const
	{
		return m_enScene == enGamePlay;
	}
private:
	EnScene m_enScene = enNonScene;
};

//GameDirectorのインスタンスを取得。
static inline GameDirector& GetGameDirector()
{
	return GameDirector::GetInstance();
}



