#pragma once
class Level;
class Guzai;
class Player;
class PlayerGene;
class SkinModelRender;
class Kitchen;
class Arrow;

class GuzaiOkiba : public IGameObject
{
private:
	Vector3 m_guzaiOkibaPos[8];											//具材置き場の座標
	Vector3 m_guzaiOkibaScale = Vector3::One;							//具材置き場の拡大率
	Vector3 m_targetPos01 = Vector3::Zero;								//1Pがアクセス可能な具材置き場の座標
	Vector3 m_targetPos02 = Vector3::Zero;								//2Pがアクセス可能な具材置き場の座標
	Vector3 m_Arrow01Pos = { 0.0f,-1000.0f,0.0f };						//1P用の矢印の座標
	Vector3 m_Arrow02Pos = { 0.0f,-1000.0f,0.0f };						//2P用の矢印の座標
	Vector3 m_Arrow1Scale = Vector3::One;								//1P用の矢印の拡大率
	Vector3 m_Arrow2Scale = Vector3::One;								//2P用の矢印の拡大率
	Vector3 m_playerPos[2];												//プレイヤーの座標
	int m_guzaiType = 0;												//具材の番号(種類)
	int m_targetNo01 = 9;													//1Pがアクセス可能な具材置き場の番号
	int m_targetNo02 = 9;													//2Pがアクセス可能な具材置き場の番号
	std::array<bool, 8> m_guzaiOkibaSet = { false };					//具材置き場にセット可能か？ trueでセット可能 falseでセット不可能
	std::array<bool, 8> m_guzaiSet = { false };							//具材置き場のセット状況 trueで置かれている falseで置かれていない
	bool m_targeted01 = false;											//1Pがターゲット中か？
	bool m_targeted02 = false;											//2Pがターゲット中か？
	std::array<float, 8> m_distance = { 0.0f };							//プレイヤーと具材置き場の距離
	float m_targetDistance01;											//1Pがアクセス可能な具材置き場の座標と1Pの距離
	float m_targetDistance02;											//2Pがアクセス可能な具材置き場の座標と2Pの距離
	float m_Arrow1ScaleSize = 1.0f;

public:
	GuzaiOkiba() {};
	~GuzaiOkiba();

	/**
	 * @brief 8つの具材置き場のポップと位置の確定
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 近い具材置き場のターゲティングとそれに伴う矢印のポップ
	*/
	void Update();

	/**
	 * @brief 受け取った2つのベクトルの距離を返す関数
	 * @param v1 地点１
	 * @param v2 地点２
	 * @return 2地点間の距離
	*/
	float TargetDistance(Vector3 v1, Vector3 v2);

	/**
	 * @brief プレイヤーと具材置き場の距離を測る関数
	*/
	void PlayerDistance();

	/**
	 * @brief 具材置き場とプレイヤーの距離を測り、範囲内の一つの具材置き場にのみアクセス可能にする。
	*/
	void Targeted();

	/**
	 * @brief 具材置き場のセット可能状況を入手
	 * @param num 具材置き場の番号を入力(0～7)
	 * @return セット状況を得る
	*/
	bool FindKitchenSet(int num) { return m_guzaiOkibaSet[num]; }

	/**
	 * @brief 具材置き場の状態を入手
	 * @param num 具材置き場の番号を入力(0～7)
	 * @return TRUE＝置かれている、FALSE＝置かれていない
	*/
	bool FindGuzaiSet(int num) { return m_guzaiSet[num]; }

	/**
	 * @brief 具材置き場の状態を変更したことを伝える関数
	 * @param num 具材置き場の番号を入力(0～7)
	 * @param isPut 具材セット完了でtrue 具材取り出しでfalse
	*/
	void GuzaiSet(int num, bool isPut) { m_guzaiSet[num] = isPut; }

	/**
	 * @brief 具材置き場の座標を入手
	 * @param num 引数に具材置き場の番号を入力(0～7)
	 * @return 置き場の座標
	*/
	Vector3 GetKitchenPos(int num) { return m_guzaiOkibaPos[num]; }

private:
	Player* m_player00 = nullptr;
	Player* m_player01 = nullptr;
	std::array<SkinModelRender*, 8> m_guzaiOkibaRender = { nullptr };
	Arrow* m_arrow00 = nullptr;
	Arrow* m_arrow01 = nullptr;
};

