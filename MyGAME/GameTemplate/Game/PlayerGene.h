#pragma once
class Player;

class PlayerGene : public IGameObject
{
private:
	Vector3 m_player01Pos = { 1000.0f,0.0f,0.0f };									//プレイヤー1の初期位置
	Vector3 m_player02Pos = { -1000.0f, 0.0f, 0.0f };								//プレイヤー2の初期位置
	Quaternion m_player01Rot = Quaternion::Identity;								//プレイヤー1の回転							
	Quaternion m_player02Rot = Quaternion::Identity;								//プレイヤー2の回転
	
	bool m_isWorking = true;														//プレイヤー生成器が稼働中か　稼働中ならプレイヤー生成が終わっていない

public:
	~PlayerGene();

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
	bool GetPlayerGeneState() const { return m_isWorking; }

private:
	std::array<Player*, 2> m_player = { nullptr };				//出現させるプレイヤーモデル
};