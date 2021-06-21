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
	 * @brief プレイヤー生成器電源状態
	 * @return true = 動いてる, false = 止まってる。
	*/
	bool GetPlayerGeneState() { return m_isWorking; }

private:
	Player* m_player[2] = { nullptr };
};

