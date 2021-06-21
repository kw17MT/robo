#pragma once

/**
 * @brief 皿の移動やその向きを設定する。
*/
class DishManager
{
private:
	const int m_dishNum = 36;
	bool m_isSpeedUp = false;											//皿の移動速度をスピードアップさせるか
	float m_timeTillSpeedUp = 0.0f;										//スピードアップまでの時間
	float m_activeTimeForSpeedUp = 0.0f;								//スピードアップの有効時間
	int m_completedChangeDirectionNum = 0;								//皿の向きを反転させた回数（皿の枚数分することを感知するため）

	bool m_letChangeDirection = false;									//皿の動く向きを変更させるか
		
	static DishManager* instance;										//DishManagerのインスタンス。
public:
	/**
	 * @brief DishManagerのインスタンスを得る
	 * @return 
	*/
	static DishManager& GetInstance()
	{
		static DishManager instance;
		return instance;
	}

	/**
	 * @brief DishManagerのインスタンスを作成する。
	*/
	void CreateInstance()
	{
		instance = new DishManager;
	}

	/**
	 * @brief DishManagerのインスタンスを破棄する。
	*/
	void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief スピードアップを実行するか返す。
	 * @return TRUE＝スピードアップ、FALSE＝普通のスピード
	*/
	bool GetSpeedUpState()
	{
		return m_isSpeedUp; 
	}

	/**
	 * @brief スピードアップしているかの状態を逆にする。
	*/
	void SwitchSpeedUpState()
	{
		//スピードアップ中ならば
		if (m_isSpeedUp == true) {
			//通常スピードに
			m_isSpeedUp = false;
		}
		//通常スピードならば
		else {
			//スピードアップに
			m_isSpeedUp = true;
		}
	}

	/**
	 * @brief スピードアップまでの時間を設定する
	 * @param timeTillSpeedUp スピードアップまでの時間
	*/
	void SetTimeTillSpeedUp(float timeTillSpeedUp)
	{
		m_timeTillSpeedUp = timeTillSpeedUp;
	}

	/**
	 * @brief スピードアップの有効時間を設定
	 * @param time スピードアップの有効時間
	*/
	void SetActiveTimeForSpeedUp(float time)
	{
		m_activeTimeForSpeedUp = time;
	}

	/**
	 * @brief スピード
	 * @return 
	*/
	float GetTimeTillSpeedUp()
	{
		return m_timeTillSpeedUp;
	}

	/**
	 * @brief スピードアップの有効時間を返す
	 * @return スピードアップの有効時間
	*/
	float GetActiveTimeForSpeedUp()
	{
		return m_activeTimeForSpeedUp;
	}

	/**
	 * @brief スピードアップまでの時間をゲームタイムで減らしていく
	 * @param gameTime ゲームタイム
	*/
	void DecreaseTimeTillSpeedUp(float gameTime)
	{
		m_timeTillSpeedUp -= gameTime;
	}

	/**
	 * @brief スピードアップの有効時間をゲームタイムで減らしていく
	 * @param gameTime ゲームタイム
	*/
	void DecreaseActiveTimeForSpeedUp(float gameTime) {
		m_activeTimeForSpeedUp -= gameTime;
	}

	void SetOrderChangeDirection(bool changeOrder)
	{
		m_letChangeDirection = changeOrder;
	}

	bool GetOrderedDirection()
	{
		return m_letChangeDirection;
	}

	void AddCompletedChangeDirectionNum()
	{
		m_completedChangeDirectionNum++;
	}

	bool JudgeChangedAllDirection()
	{
		return m_dishNum == m_completedChangeDirectionNum;
	}
};

