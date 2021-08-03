#pragma once
class SpriteRender;
class FixedUI;

class Result : public IGameObject
{
private:
	Vector2 m_position = Vector2::Zero;						//画像の位置
	Vector2 m_pivot = {0.5f,0.5f};							//画像の中心
	Vector2 m_scale = { 1.0f,1.0f };										//画像の拡大率
	const Vector3 m_center = { 0.0f,0.0f,0.0f };			//中央表示用の位置
	const Vector3 m_left = { 450.0f, 0.0f, 0.0f };			//左表示用の位置
	const Vector3 m_right = { -450.0f, 0.0f, 0.0f };		//右表示用の位置
	Vector4 m_color = { 1.0f,1.0f,1.0f, 0.0f};				//初期カラー
	Quaternion m_rotation = Quaternion::Identity;			//画像の回転

	const int m_wideth = 512;								//画像の縦
	const int m_height = 512;								//画像の縦
	int m_spriteNum = 0;										//どのスプライトにするかを決めるための整数値
	int m_positionNum = 0;										//どの位置に表示するかを決めるための整数値
	
	float m_alpha = 0.0f;									//アルファ用変数
	float m_scaleRate = 3.0f;								//拡大率用変数
	bool m_isReach3Miss = false;							//どちらかのMISSが3つたまった時にTRUEにする

public:
	~Result();
	
	/**
	 * @brief 画像の初期化
	*/
	bool Start();

	/**
	 * @brief 縮小と透明度のアップ
	*/
	void Update();

	/**
	 * @brief 新しい座標の設定
	*/
	void SetPosition(const Vector2& pos) { m_position = pos; }

	/**
	 * @brief 新しい中心の設定
	*/
	void SetPivot(const Vector2& piv) { m_pivot = piv; }

	/**
	 * @brief 新しい拡大率の設定
	*/
	void SetScale(const Vector2& scl) { m_scale = scl; }

	/**
	 * @brief 新しい回転の設定
	*/
	void SetRotation(const Quaternion& rot) { m_rotation = rot; }

	/**
	 * @brief どちらかのプレイヤーが3ミスになったか
	*/
	void Reach3Miss(bool state) { m_isReach3Miss = state; }

	/**
	 * @brief 呼び出しもとで画像の種類を決定する
	*/
	void SetSprite(int No) { m_spriteNum = No; }

	/**
	 * @brief 位置番号を指定して座標を設定する
	*/
	void SetSpritePos(int posNo) { m_positionNum = posNo; }

	/**
	 * @brief ddsの振り分け関数
	*/
	void DecideDDS();

	/**
	 * @brief 位置を決定する
	*/
	void DecidePos();

private:
	FixedUI* m_ui = nullptr;								//タイムアップしているか確認するためのインスタンス
	SpriteRender* m_spriteRender = nullptr;					//勝ち、負け、引き分けの画像
};

