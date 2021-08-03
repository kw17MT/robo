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
	
	int m_movingTime = 30;											//ゴミを入れた後どのくらいの時間動くか
	int m_trashcanNo = 0;											//ゴミ箱の番号、１が左、２が右

	const float m_distance = 130.0f;								//ゴミ箱がプレイヤーを識別する距離

	bool m_canTrash = false;										//プレイヤーが近いとTRUEにして捨てることができるようにする。
	bool m_targetUp = true;											//ターゲットアイコンを浮遊させるよう
	bool m_isMoving = false;										//ゴミ箱を動かさせるか

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
	Vector3 GetPosition() const { return m_position; }

	/**
	 * @brief ゴミ箱の番号の設定　０が左　１が右
	 * @param num ゴミ箱の番号
	*/
	void SetTrashCanNum(int num) { m_trashcanNo = num; }

	/**
	 * @brief ゴミ箱は現在何かを捨てられる状態かを返す
	 * @return ゴミ箱の状態
	*/
	bool GetCanTrash() const { return m_canTrash; }

	/**
	 * @brief ２地点間の距離を求める
	 * @param v1 地点１
	 * @param v2 地点２
	 * @return ２地点間の距離
	*/
	float CalcDistance(Vector3 v1, Vector3 v2)
	{
		Vector3 v3 = v1 - v2;
		float ans = v3.Length();
		return ans;
	}

	/**
	 * @brief ゴミ箱は何かを捨てられてリアクションをしているか
	 * @param state TRUE＝している
	*/
	void ChangeMovingState(bool state) { m_isMoving = state; }
	
	/**
	 * @brief ゴミを入れられたらリアクションをする
	*/
	void LetStartMoving();

	/**
	 * @brief 矢印の拡大率を変化させる
	*/
	void ChangeArrowScale();

	/**
	 * @brief 矢印の浮遊処理
	*/
	void FloatArrow();

private:
	Player* m_player = nullptr;								//プレイヤーの情報取得用
	SkinModelRender* m_skinModelRender = nullptr;			//ゴミ箱のモデル用
	SkinModelRender* m_arrow = nullptr;						//矢印のモデル用
	PlayerGene* m_playerGene = nullptr;						//プレイヤー生成器の情報取得用
};