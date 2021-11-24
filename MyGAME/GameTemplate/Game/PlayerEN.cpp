#include "stdafx.h"
#include "PlayerEN.h"
#include "SpriteRender.h"

namespace
{
	const float MAX_EN = 100.0f;				//最大エネルギー量
	const float EN_REDUCE_AMOUNT = 0.5f;
}

PlayerEN::~PlayerEN()
{
	//EN表示画像の削除
	DeleteGO(m_spriteRender);
}

bool PlayerEN::Start()
{
	//画像インスタンスの生成
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/EN.dds", 500, 100);
	//位置設定
	m_spriteRender->SetPosition(m_screenPos);

	return true;
}

void PlayerEN::Update()
{
	//RB1ボタンが押されたら
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//エネルギーが残っているならば
		if (m_isPlayerEnRemain) {
			//定数ずつ消費
			m_playerEN -= EN_REDUCE_AMOUNT;
			if (m_playerEN <= 0.0f)
			{
				m_playerEN = 0.0f;
			}
		}
		//一度すべてのENを使い切っているならば（オーバーヒート中）
		else
		{
			//定数ずつ回復
			m_playerEN += 0.5f;
			if (m_playerEN >= 100.0f)
			{
				m_playerEN = 100.0f;
			}
		}
	}
	//RB1ボタンは押されていない
	else
	{
		//定数ずつ回復
		m_playerEN += 0.5f;
		if (m_playerEN >= 100.0f)
		{
			m_playerEN = 100.0f;
		}
	}

	//エネルギーをすべて使い切ったら
	if (m_playerEN <= 0.0f)
	{
		m_playerEN = 0.0f;
		//エネルギーがない
		m_isPlayerEnRemain = false;
	}
	//回復しきったら
	if (m_playerEN >= MAX_EN)
	{
		//エネルギーがある
		m_isPlayerEnRemain = true;
	}

	//エネルギーの残量に従って画像の大きさを設定する。
	m_spriteRender->SetSpriteSizeRate(1.0f - m_playerEN / MAX_EN);
}