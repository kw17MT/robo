#pragma once
#include "SkinModelRender.h"

/**
 * @brief 具材置き場の近くにプレイヤーがいると出現する。
 * 見た目のサポートを行うオブジェクト
*/
class Arrow : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;
	int m_timer = 0;
	float m_setYPos = 0.0f;
	bool m_scaleMaxFlag = false;
	bool m_scaleMinFlag = false;
public:
	Arrow() {}
	~Arrow();

	/**
	 * @brief 矢印のモデルの初期化
	 * @return true
	*/
	bool Start();

	/**
	 * @brief 上下に動きながら浮遊させる
	*/
	void Update();
	
	/**
	 * @brief 矢印を座標によって上下させる。
	*/
	void Float();

	/**
	 * @brief 拡大処理
	*/
	void Expansion();

	/**
	 * @brief 縮小処理
	*/
	void Shrink();

	/**
	 * @brief 新しいポジションの設定
	 * @param pos 新しい位置座標
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief 新しいスケールの設定
	 * @param scale 新しいスケール
	*/
	void SetScale(Vector3 scale) { m_scale = scale; }

	/**
	 * @brief 拡大率が最大になったらのフラグを取得する
	 * @return 拡大率は最大になったか
	*/
	bool GetScaleMax() const { return m_scaleMaxFlag; }

	/**
	 * @brief 拡大率が最小になったらのフラグを取得する
	 * @return 拡大率は最小になったか
	*/
	bool GetScaleMin() const { return m_scaleMinFlag; }

private:
	SkinModelRender* m_skinModelRender = nullptr;		//矢印のモデル
};

