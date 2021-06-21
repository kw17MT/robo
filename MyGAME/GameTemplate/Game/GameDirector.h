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
	/**
	 * @brief GameDirectorのインスタンスを取得。
	 * @return GameDirectorのインスタンス
	*/
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}

	//ゲームシーンを設定。

	/**
	 * @brief 
	 * @param enScene 
	*/
	void SetGameScene(EnScene enScene)
	{
		m_enScene = enScene;
	}

	/**
	 * @brief 今がどんなシーンが取得。
	 * @return 今のシーン状況
	*/
	EnScene GetGameScene() const
	{
		return m_enScene;
	}

	/**
	 * @brief カウントダウン中？
	 * @return ０＝カウントダウン中
	*/
	bool GetIsGameCountDown() const
	{
		return m_enScene == enGameCountDown;
	}

	/**
	 * @brief ゲームプレイ中？
	 * @return １＝プレイ中
	*/
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