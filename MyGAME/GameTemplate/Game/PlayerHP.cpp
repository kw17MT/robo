#include "stdafx.h"
#include "PlayerHP.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "SoundSource.h"

namespace
{
	const float BULLET_DAMAGE = 1.0f;			//弾丸のダメージ量
	const float MISSILE_DAMAGE = 10.0f;			//ミサイルのダメージ量
	const float RASER_DAMAGE = 20.0f;			//レールガンのダメージ量
	const float MAX_HP = 100.0f;				//最大HP 
	const float FONT_SIZE = 0.5f;				//文字サイズ
	const float DECREASE_RATE = 3.0f;
}

PlayerHP::~PlayerHP()
{
	//画像インスタンスの削除
	DeleteGO(m_spriteRender);
}

void PlayerHP::ApplyDamage(EnPlayerDamageTypes enemyDamageType)
{
	switch (enemyDamageType)
	{
	//弾丸に攻撃された時
	case enEnemyBullet:
		//弾丸ダメージ適用
		m_playerHp -= BULLET_DAMAGE;
		break;
	//ミサイルに攻撃された時
	case enEnemyMissile:
		//ミサイルダメージ適用
		m_playerHp -= MISSILE_DAMAGE;
		break;
	//レールガンに攻撃された時
	case enEnemyRaser:
		//レールガンダメージ適用
		m_playerHp -= RASER_DAMAGE;
		break;
	}
}

bool PlayerHP::Start()
{
	//画像インスタンス生成
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/HP.dds", 500, 100);
	//画像の位置を固定
	m_spriteRender->SetPosition(m_screenPos);

	////文字インスタンス生成
	//m_fontRender = NewGO<FontRender>(0);
	////空初期化
	//m_fontRender->SetText(L"");
	////大きさ設定
	//m_fontRender->SetScale(FONT_SIZE);
	////位置設定
	//m_fontRender->SetPosition({ m_screenPos.x, m_screenPos.y });
	return true;
}

void PlayerHP::Update()
{
	//プレイヤーの残りHPを文字列に変換
	//std::wstring playerHp = std::to_wstring((int)m_playerHp);
	//m_fontRender->SetText(playerHp.c_str());

	//HPがなくなったら
	if (m_playerHp <= 0.0f)
	{
		m_playerHp = 0;
		//プレイヤーは生きていない
		m_isPlayerAlive = false;
	}

	//実際のHPの値と、HPゲージのサイズが違うとき
	if (m_playerHp != m_hpSize)
	{
		//実際のHPとHPバーの差を計算する
		float def = m_hpSize - m_playerHp;
		//その差を定数で割り徐々に差を埋めていく
		m_hpSize -= def / DECREASE_RATE;
		//実際のHPより、ゲージのサイズが小さくないように調節
		if (m_playerHp >= m_hpSize)
		{
			m_hpSize = m_playerHp;
		}
	}

	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= 0.7f
		&& !m_isSoundBuzzer)
	{
		CSoundSource* buzzerSE = NewGO<CSoundSource>(0);
		buzzerSE->Init(L"Assets/sound/HpBuzzer.wav", false);
		buzzerSE->SetVolume(0.5f);
		buzzerSE->Play(false);

		m_isSoundBuzzer = true;
	}
	m_spriteRender->SetSpriteSizeRate(HpSize);

	//残りHP量に従って画像の大きさを変更する
	//m_spriteRender->SetSpriteSizeRate(1.0f - m_playerHp / MAX_HP);
}