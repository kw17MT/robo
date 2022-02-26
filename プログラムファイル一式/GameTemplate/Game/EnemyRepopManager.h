#pragma once
class EnemyRepopManager
{
private:
	static EnemyRepopManager* instance;			//シングルトンインスタンス
	bool m_shouldRepopEnemy = false;			//次の敵をポップさせるかどうか
	int m_popedNum = 0;							//現在ポップしている敵の数
	int m_killedNum = 0;						//倒された敵の数
public:
	/**
	 * @brief インスタンスを作成する
	*/
	static void CreateInstance()
	{
		instance = new EnemyRepopManager;
	}

	/**
	 * @brief インスタンスを取得する
	 * @return このインスタンス
	*/
	static EnemyRepopManager& GetInstance()
	{
		static EnemyRepopManager instance;
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
	 * @brief ポップさせた敵の数を設定する
	 * @param state ポップさせた敵の数
	*/
	void SetRepopEnemy(bool state)
	{
		m_shouldRepopEnemy = state;
	}

	/**
	 * @brief 敵をリポップさせるべきかどうか判定する
	 * @return リポップさせるべきかどうか
	*/
	bool ShouldRepopEnemy()
	{
		//ポップさせた敵が0でなく、ポップさせた敵がすべて倒されていたら
		if (m_popedNum != 0
			&& m_popedNum == m_killedNum)
		{
			//敵の情報をリセットする
			m_popedNum = 0;
			m_killedNum = 0;
			return true;
		}
		return false;
	}

	/**
	 * @brief ポップさせた敵の数を設定する
	 * @param num ポップさせた敵の数
	*/
	void SetPopedEnemyNum(int num)
	{
		m_popedNum = num;
	}

	/**
	 * @brief 倒された敵の数を加える
	*/
	void AddKilledEnemyNum()
	{
		m_killedNum++;
	}

	/**
	 * @brief 全てのパラメーターをリセットする
	*/
	void ResetParam()
	{
		m_shouldRepopEnemy = false;
		m_popedNum = 0;
		m_killedNum = 0;
	}
};

