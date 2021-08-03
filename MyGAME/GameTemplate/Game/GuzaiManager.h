#pragma once

enum EnOrder {
	enDoNothing,
	enStartRefill,
	enOrderNum
};

class GuzaiManager
{
private:
	static GuzaiManager* instance;
	
	int m_emptyDishNum = 0;							//空の皿の数
	int m_refilledGuzaiNum = 0;						//補充した具材の数
	const int m_numToStartRefill = 5;				//補充を開始する具材の数

	EnOrder m_enOrder = enDoNothing;
public:
	/**
	 * @brief インスタンスの取得
	 * @return 具材管理のインスタンス
	*/
	static GuzaiManager& GetInstance()
	{
		static GuzaiManager instance;
		return instance;
	}

	/**
	 * @brief インスタンスの作成
	*/
	static void CreateInstance()
	{
		instance = new GuzaiManager;
	}

	/**
	 * @brief インスタンスの削除
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief 関数の呼び出し時、空の皿が補充開始する数に達していたら、開始の合図行う。
	*/
	void JudgeToOrderRefill()
	{
		if (m_emptyDishNum >= m_numToStartRefill) {
		m_enOrder = enStartRefill;
		}
	}

	/**
	 * @brief 何もしないようにという命令に設定する。
	*/
	void SetNothingOrder()
	{
		m_enOrder = enDoNothing;
	}

	/**
	 * @brief 空の皿の数を数える
	*/
	void AddEmptyDishNum() 
	{
		m_emptyDishNum++;
	}

	/**
	 * @brief 空の皿の数を取得する
	 * @return 空の皿の数
	*/
	int GetEmptyDishNum() const
	{
		return m_emptyDishNum;
	}

	/**
	 * @brief 補充が完了した回数を記録する（インクリメント）
	*/
	void AddRefilledGuzaiNum()
	{
		m_refilledGuzaiNum++;
	}

	/**
	 * @brief 補充が完了した回数を返す。
	 * @return 補充が完了した回数
	*/
	int GetRefilledGuzaiNum() const
	{
		return m_refilledGuzaiNum;
	}

	/**
	 * @brief 空の皿の数と補充した回数を０でリセットする。
	*/
	void ResetParamAboutDishAndRefill()
	{
		m_refilledGuzaiNum = 0;
		m_emptyDishNum = 0;
	}

	/**
	 * @brief 補充を命令されているか
	 * @return TRUE＝命令された　FALSE＝されてない
	*/
	bool IsOrderedRefill() const
	{
		if (m_emptyDishNum >= m_numToStartRefill) {
			return m_enOrder == enStartRefill;
		}

		return false;
	}
};

static inline GuzaiManager& GetGuzaiManager()
{
	return GuzaiManager::GetInstance();
}