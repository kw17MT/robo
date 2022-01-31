#pragma once

/**
 * @brief ステージの種類
*/
enum EnStage
{
	en1_1,
	en1_2,
	enStageNum
};

/**
 * @brief ステージのマネージャー
 * 現在は未使用
*/
class StageManager : public IGameObject
{
private:
	static StageManager* instance;
	EnStage m_stage = en1_1;
public:
	StageManager() {};
	/**
	 * @brief インスタンスを作る
	*/
	static void CreateInstance()
	{
		instance = new StageManager;
	}

	/**
	 * @brief インスタンスを取得する
	 * @return シングルトンインスタンス
	*/
	static StageManager& GetInstance()
	{
		static StageManager instance;
		return instance;
	}

	/**
	 * @brief インスタンスを削除する。
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief 現在のステージを取得する
	 * @return 現在のステージ番号
	*/
	EnStage GetStage() const
	{
		return m_stage;
	}

	/**
	 * @brief 使用するステージを設定する
	 * @param stage 使用するステージ番号
	*/
	void SetStage(EnStage stage)
	{
		m_stage = stage;
	}
};

