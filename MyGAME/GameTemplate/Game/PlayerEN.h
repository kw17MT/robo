#pragma once
class SpriteRender;

class PlayerEN : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -300.0f, 0.0f };				//固定座標

	float m_playerEN = 100.0f;									//プレイヤーの最大エネルギー
	bool m_isPlayerEnRemain = true;								//プレイヤーのエネルギーは残っているか
public:
	/**
	 * @brief コンストラクタ
	*/
	PlayerEN() {}

	/**
	 * @brief デストラクタ
	*/
	~PlayerEN();

	/**
	 * @brief インスタンス生成時に一度だけ呼ばれる関数
	 * @return 
	*/
	bool Start();

	/**
	 * @brief 毎フレーム呼ばれる関数
	*/
	void Update();

	/**
	 * @brief プレイヤーのエネルギーは残っているかの確認関数
	 * @return TRUE残っている　FALSEない
	*/
	bool GetIsPlayerENRemain() { return m_isPlayerEnRemain; }
private:
	SpriteRender* m_spriteRender = nullptr;						//EN表示用画像
};

