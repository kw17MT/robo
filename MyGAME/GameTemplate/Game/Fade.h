#pragma once
#include"SpriteRender.h"

//ステート
enum EnState {
	enState_In,		//フェードイン
	enState_Out,	//フェードアウト
	enState_Wait,	//待機
};


class Fade : public IGameObject
{
private:	
	SpriteRender* m_spriteRender = nullptr;					//画像
	float m_alpha = 0.0f;									//アルファ値
public:
	Fade();
	~Fade();
	//更新処理。
	void Update();

	/**
	 * @brief アルファ値を返す
	 * @return 透明度
	*/
	float GetAlpha() const
	{
		return m_alpha;
	}
	
	/**
	 * @brief ステート設定
	 * @param state 状態
	*/
	void SetState(EnState state)
	{
		m_state = state;
	}
	
	/**
	 * @brief ステート取得
	 * @return ステート
	*/
	EnState GetState() const
	{
		return m_state;
	}
private:
	EnState m_state;	//ステート
};
