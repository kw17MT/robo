#include "stdafx.h"
#include "PlayerHP.h"
#include "SpriteRender.h"
#include "FontRender.h"
#include "SoundSource.h"

namespace
{
	const float BULLET_DAMAGE = 3.0f;			//弾丸のダメージ量
	const float MISSILE_DAMAGE = 10.0f;			//ミサイルのダメージ量
	const float RASER_DAMAGE = 20.0f;			//レールガンのダメージ量
	const float MAX_HP = 100.0f;				//最大HP 
	const float DECREASE_RATE = 3.0f;
	const float SE_VOLUME = 0.4f;
	const float HPBAR_DANGER_SIZE = 0.7f;
	const float DISAPPEAR_RATE = 0.01f;
}

PlayerHP::~PlayerHP()
{
	//画像インスタンスの削除
	DeleteGO(m_spriteRender);
	DeleteGO(m_damagedSprite);
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
	m_damagedSpriteAlpha = 0.7f;
	SoundDamagedSE(rand() % 2);
}

bool PlayerHP::Start()
{
	//画像インスタンス生成
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->InitGauge("Assets/Image/PlayerUI/HP.dds", 500, 100);
	//画像の位置を固定
	m_spriteRender->SetPosition(m_screenPos);

	m_damagedSprite = NewGO<SpriteRender>(0);
	m_damagedSprite->Init("Assets/Image/Damaged/damaged.dds", 1280, 720);
	m_damagedSprite->SetScale(Vector3::One);
	m_damagedSprite->SetAlpha(m_damagedSpriteAlpha);

	return true;
}

void PlayerHP::SoundDamagedSE(int soundNo)
{
	CSoundSource* damaged = NewGO<CSoundSource>(0);
	switch (soundNo)
	{
	case 0:
		damaged->Init(L"Assets/sound/damaged.wav", false);

		break;
	case 1:
		damaged->Init(L"Assets/sound/damaged1.wav", false);

		break;
	}
	damaged->SetVolume(SE_VOLUME);
	damaged->Play(false);
}

float PlayerHP::GetHPBarAmountInDanger()
{
	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= HPBAR_DANGER_SIZE)
	{
		//残りHPバーは危険状態のHPバーの何割か
		float remainAmount = 1.0f - (1.0f - HpSize) / (1.0f - HPBAR_DANGER_SIZE);
		return remainAmount;
	}
	else
	{
		return 0.0f;
	}
}

void PlayerHP::Update()
{
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

	//残りHPからゲージの大きさを計算する
	float HpSize = 1.0f - m_hpSize / MAX_HP;
	if (HpSize >= HPBAR_DANGER_SIZE)
	{
		//ブザーを鳴らしていなかったら
		if (!m_isSoundBuzzer)
		{
			//音を鳴らす
			CSoundSource* buzzerSE = NewGO<CSoundSource>(0);
			buzzerSE->Init(L"Assets/sound/HpBuzzer.wav", false);
			buzzerSE->SetVolume(SE_VOLUME);
			buzzerSE->Play(false);

			m_isSoundBuzzer = true;
		}

		////残りHPバーは危険状態のHPバーの何割か
		//float remainAmount = 1.0f - (1.0f - HpSize) / (1.0f - HPBAR_DANGER_SIZE);
		////設定されているモノクロ率より、今回計算した残り割合が大きければ
		//if (remainAmount > RenderingEngine::GetInstance()->GetMonochromeRate())
		//{
		//	//モノクロ率をセット
		//	RenderingEngine::GetInstance()->SetMonochromeRate(remainAmount);
		//}
	}

	m_damagedSpriteAlpha -= DISAPPEAR_RATE;
	if (m_damagedSpriteAlpha <= 0.0f)
	{
		m_damagedSpriteAlpha = 0.0f;
	}
	m_damagedSprite->SetAlpha(m_damagedSpriteAlpha);
	m_spriteRender->SetSpriteSizeRate(HpSize);
}