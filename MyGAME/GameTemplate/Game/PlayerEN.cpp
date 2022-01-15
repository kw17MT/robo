#include "stdafx.h"
#include "PlayerEN.h"
#include "SpriteRender.h"
#include "SoundSource.h"

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
				//エネルギーがない
				m_isPlayerEnRemain = false;

				CSoundSource* downSE = NewGO<CSoundSource>(0);
				downSE->Init(L"Assets/sound/EnDown.wav", false);
				downSE->SetVolume(0.5f);
				downSE->Play(false);
			}

			m_blinkingTime = 0.0f;
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
		if (m_playerEN >= MAX_EN)
		{
			m_playerEN = MAX_EN;
		}
	}

	//回復しきったら
	if (m_playerEN >= MAX_EN)
	{
		//エネルギーがある
		m_isPlayerEnRemain = true;
	}

	//エネルギーの残量に従って画像の大きさを設定する。
	m_spriteRender->SetSpriteSizeRate(1.0f - m_playerEN / MAX_EN);

	//ENがなくなってオーバーヒートしている時
	if (!m_isPlayerEnRemain)
	{
		//点滅する時間をデルタタイムで増やす
		m_blinkingTime += GameTime().GetFrameDeltaTime();
		if (m_blinkingTime >= 0.4f)
		{
			//元に戻す
			m_blinkingTime = 0.0f;
		}
		//ENゲージがついて消えたら（1サイクルしたら）
		if (m_blinkingTime >= 0.2f)
		{
			//点滅させるためにゲージを全部表示させないようにする
			m_spriteRender->SetSpriteSizeRate(1.0f);
		}
	}
}