#pragma once
class SkinModelRender;
class Player;
class PlayerGene;

class TrashCan : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;								//ゴミ箱の位置
	Vector3 m_arrowPos = Vector3::Zero;								//ゴミ箱の上に出てくる矢印の位置
	Vector3 m_trashcanScale = { 1.0f,1.0f,1.0f };					//ゴミ箱の拡大率
	Vector3 m_targetScale = Vector3::Zero;							//矢印の拡大率
	Quaternion m_rot = Quaternion::Identity;						//矢印の回転
	
	int movingTime = 30;											//ゴミを入れた後どのくらいの時間動くか
	int trashcanNo = 0;												//ゴミ箱の番号、１が左、２が右

	const float distance = 130.0f;									//ゴミ箱がプレイヤーを識別する距離

	bool canTrash = false;											//プレイヤーが近いとTRUEにして捨てることができるようにする。
	bool targetUp = true;											//ターゲットアイコンを浮遊させるよう
	bool isMoving = false;											//ゴミ箱を動かさせるか

public:
	~TrashCan();

	/**
	 * @brief ゴミ箱のモデルの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ゴミ箱のリアクションをするか判定、矢印の拡大、浮遊を表現する。
	*/
	void Update();

	/**
	 * @brief 矢印の拡大率を設定する
	 * @param scale 新しい拡大率
	*/
	void SetScale(Vector3 scale) { m_targetScale = scale; }

	/**
	 * @brief ゴミ箱の位置を設定する
	 * @param pos 新しい位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief ゴミ箱の座標を返す。
	 * @return ゴミ箱の座標
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief ゴミ箱の番号の設定　０が左　１が右
	 * @param num ゴミ箱の番号
	*/
	void SetTrashCanNum(int num) { trashcanNo = num; }

	/**
	 * @brief ゴミ箱は現在何かを捨てられる状態かを返す
	 * @return ゴミ箱の状態
	*/
	bool GetCanTrash() { return canTrash; }

	/**
	 * @brief ２地点間の距離を求める
	 * @param v1 地点１
	 * @param v2 地点２
	 * @return ２地点間の距離
	*/
	float CalcDistance(Vector3 v1, Vector3 v2);

	/**
	 * @brief ゴミ箱は何かを捨てられてリアクションをしているか
	 * @param state TRUE＝している
	*/
	void ChangeMovingState(bool state) { isMoving = state; }
	
	/**
	 * @brief ゴミを入れられたらリアクションをする
	*/
	void LetStartMoving();

private:
	Player* m_player = nullptr;
	SkinModelRender* m_skinModelRender = nullptr;
	SkinModelRender* m_arrow = nullptr;
	PlayerGene* m_playerGene = nullptr;
};