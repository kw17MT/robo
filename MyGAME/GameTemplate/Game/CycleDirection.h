#pragma once
class SpriteRender;


enum EnDirection {
	Forward,		//回転スプライト正転
	Reverse,		//回転スプライト逆転
	FixedForward,	//固定表示スプライト正転
	FixedReverse,	//固定表示スプライト逆転
};

enum EnSide {
	Left,			//左
	Right,			//右
	Up				//画面上部
};

class CycleDirection : public IGameObject
{
public:
	CycleDirection() {};
	~CycleDirection();

	/**
	 * @brief 画像の初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 画像の回転を行う
	*/
	void Update();
	
	/**
	 * @brief 正転画像に変更する。
	*/
	void ChangeSpriteForward();

	/**
	 * @brief 逆転画像に変更する。
	*/
	void ChangeSpriteReverse();

	/**
	 * @brief 固定された回転の向きを表す画像を正転用にする。
	*/
	void ChangeFixedSpriteForward();

	/**
	 * @brief 逆転用にする。
	*/
	void ChangeFixedSpriteReverse();

	/**
	 * @brief 回転方向の向きを設定する
	 * @param enDirection 新しい回転の向き
	*/
	void SetDirection(const EnDirection& enDirection) { m_direction = enDirection; }

	/**
	 * @brief 右か左かどちらに表示させるか
	 * @param enSide enSide参照
	*/
	void SetSide(const EnSide& enSide) { m_side = enSide; }
	
	/**
	 * @brief 回転の方向を返す
	 * @return 回転の方向
	*/
	EnDirection GetDirection() const { return m_direction; }

	/**
	 * @brief どちらに表示されているか返す
	 * @return 表示位置
	*/
	EnSide GetSide() const { return m_side; }

private:
	Vector4 m_finalColor = { 2.0f,2.0f,2.0f,m_alpha };	//回転の画像の色
	Vector4 m_fixedColor = { 1.0f,1.0f,1.0f,m_alpha };	//固定する画像の色
	Vector3 m_scale = { 0.5f,0.5f,1.0f };						//拡大率
	Vector3 m_position = Vector3::Zero;							//画像の位置
	Quaternion m_rotation = Quaternion::Identity;				//回転
	float m_angle = 0.0f;										//回転の率
	float m_alpha = 0.0f;										//画像の透明度
	EnDirection m_direction = Forward;							//回転の向き
	EnSide m_side = Left;										//回転の場所

	SpriteRender* m_sprite = nullptr;							//画像
};

