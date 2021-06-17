#pragma once
class Player;

class PlayerGene : public IGameObject
{
private:
	Vector3 m_player01Pos = { 1000.0f,0.0f,0.0f };									//プレイヤー1の初期位置
	Vector3 m_player02Pos = { -1000.0f, 0.0f, 0.0f };								//プレイヤー2の初期位置
	Quaternion m_player01Rot = Quaternion::Identity;								//プレイヤー1の回転							
	Quaternion m_player02Rot = Quaternion::Identity;								//プレイヤー2の回転

	int m_playerNum = 0;															//何人プレイヤーを生成したかの数
	int m_noHavingDishCounter = 0;													//空の皿の数
	int m_refilledNum = 0;															//補充できた具材の数
	int m_submitBurgerNum = 0;														//提出したバーガーの数
	int m_changeCycleNum = 0;														//何個補充したか。Dishで使用。これがmaxNum2Refillと同値になると補充完了を意味し、noHavingDish、refilledNumを０で初期化する。
	
	bool m_isGameSet = false;														//ゲームが終了したか
	bool m_isWorking = true;														//プレイヤー生成器が稼働中か　稼働中ならプレイヤー生成が終わっていない

public:
	~PlayerGene();
	
	bool Start() { return true; }

	/**
	 * @brief プレイヤーを2体出す。
	*/
	void Update();

	/**
	 * @brief プレイヤー１の位置を変える
	 * @param pos 新しい位置座標
	*/
	void SetPlayer01Pos(Vector3 pos) { m_player01Pos = pos; }

	/**
	 * @brief プレイヤー２の位置を変える
	 * @param pos 新しい位置座標
	*/
	void SetPlayer02Pos(Vector3 pos) { m_player02Pos = pos; }

	/**
	 * @brief 空の皿はいくつあるかを返す
	 * @return 空の皿の数
	*/
	int GetNoHavingDishCounter() { return m_noHavingDishCounter; }

	/**
	 * @brief 空の皿の数を1つ足す
	*/
	void AddNoHavingDishCounter() { m_noHavingDishCounter++; }

	/**
	 * @brief 空の皿の数を1つ消す　具材置き場で使う。
	*/
	void MinusNoHavingDishCounter() { m_noHavingDishCounter--; }

	/**
	 * @brief 補充完了時に空の皿を０に戻す
	*/
	void ResetNohavingDishCounter() { m_noHavingDishCounter = 0; }

	/**
	 * @brief 補充完了した皿の数を返す
	 * @return 補充完了した皿の数
	*/
	int GetRefilledNum() { return m_refilledNum; }

	/**
	 * @brief 補充完了した皿の数を一つ足す
	*/
	void AddRefilledNum() { m_refilledNum++; }

	/**
	 * @brief 補充完了時に補充した皿を０に戻す
	*/
	void ResetRefilledNum() { m_refilledNum = 0; }

	/**
	 * @brief 提出したバーガーの数を1つ足す
	*/
	void AddSubmitBurgerNum() { m_submitBurgerNum++; }

	/**
	 * @brief 提出したバーガーの数を返す
	 * @return 提出したバーガーの数
	*/
	int GetSubmitBurgerNum() { return m_submitBurgerNum; }

	/**
	 * @brief 提出したハンバーガーの数をリセットする。
	*/
	void ResetSubmitBurgerNum() { m_submitBurgerNum = 0; }	

	/**
	 * @brief 流れを変えた回数をインクリメントする。
	*/
	void AddChangeCycleNum() { m_changeCycleNum++; }

	/**
	 * @brief 流れを変えた回数を返す
	 * @return 流れを変えた回数
	*/
	int GetChangeCycleNum() { return m_changeCycleNum; }

	/**
	 * @brief 流れを変えた回数をリセットする。
	*/
	void ResetChangeCycleNum() { m_changeCycleNum = 0; }

	/**
	 * @brief プレイヤー生成器電源状態
	 * @return true = 動いてる, false = 止まってる。
	*/
	bool GetPlayerGeneState() { return m_isWorking; }

	/**
	 * @brief ゲームは終わっているか返す
	 * @return TRUE＝終わっている
	*/
	bool GetIsGameSet() { return m_isGameSet; }

	/**
	 * @brief ゲームが終わっているかどうかを設定する。
	 * @param state ゲームが終わっているかどうか
	*/
	void SetIsGameSet(bool state) { m_isGameSet = state; }

private:
	Player* m_player[2] = { nullptr };
};

