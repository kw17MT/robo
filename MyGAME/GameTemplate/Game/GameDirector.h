#pragma once

/**
 * @brief ゲームシーン
*/
enum EnGameScene
{
	enTitle,				//タイトル
	enExit,					//ゲーム終了
	enLaunchingPlayer,		//プレイヤー出撃演出中
	enInGame,				//ゲーム中
	enGameClear,			//ゲームクリアシーン中
	enGameOver,				//ゲームオーバー演出
	enAfterGameOver,		//ゲームオーバー演出終了後
	enToTitle				//タイトルへ戻る
};

class GameDirector
{
private:
	static GameDirector* instance;				//シングルトンインスタンス
	EnGameScene enGameScene = enTitle;			//現在のゲームシーン
public:
	/**
	 * @brief インスタンスを作成する
	*/
	static void CreateInstance()
	{
		instance = new GameDirector;
	}

	/**
	 * @brief インスタンスを取得する
	 * @return このインスタンス
	*/
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
		return instance;
	}

	/**
	 * @brief インスタンスを削除する
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief 現在のゲームシーンを取得する
	 * @return ゲームシーン
	*/
	EnGameScene GetGameScene()
	{
		return enGameScene;
	}

	/**
	 * @brief 現在のゲームシーンを設定する
	 * @param scene 現在のゲームシーン
	*/
	void SetGameScene(EnGameScene scene)
	{
		enGameScene = scene;
	}
};

