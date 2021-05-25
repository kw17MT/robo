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
	SpriteRender* m_spriteRender;
	float m_alpha = 0.0f;	//アルファ値
public:
	Fade();
	~Fade();
	//更新処理。
	void Update();

	

	float GetAlpha()
	{
		return m_alpha;
	}

	/// <summary>
	/// ステート設定
	/// </summary>
	/// <param name="state">ステート</param>
	void SetState(EnState state)
	{
		m_state = state;
	}
	/// <summary>
	/// ステート取得
	/// </summary>
	/// <returns>ステート</returns>
	EnState GetState() const
	{
		return m_state;
	}
private:
	EnState m_state;	//ステート
};
