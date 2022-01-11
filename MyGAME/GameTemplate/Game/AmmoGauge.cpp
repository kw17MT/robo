#include "stdafx.h"
#include "AmmoGauge.h"
#include "SpriteRender.h"
#include "FontRender.h"

namespace
{
	const float BULLET_RELOAD_COMPLETE_TIME = 3.0f;				//弾のリロード時間
	const float MISSILE_RELOAD_COMPLETE_TIME = 5.0f;			//ミサイルのリロード時間
	const float RASER_RELOAD_COMPLETE_TIME = 10.0f;				//レールガンのリロード時間
	const int GAUGE_SCALE_X = 160;								//ゲージのX拡大率
	const int GAUGE_SCALE_Y = 12;								//ゲージのY拡大率
	const float FONT_ADJUST_AMOUNT_X = 90.0f;					//文字の位置の調整変数
	const float FONT_ADJUST_AMOUNT_Y = 15.0f;					//文字の位置の調整変数
}

AmmoGauge::~AmmoGauge()
{
	//画像の削除
	DeleteGO(m_spriteRender);
	DeleteGO(m_fontRender);
}

bool AmmoGauge::Start()
{
	//ゲージの生成
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->InitGauge("Assets/Image/gaugeTexture/ammoGauge.dds", GAUGE_SCALE_X, GAUGE_SCALE_Y);
	m_spriteRender->SetPivot(m_pivot);

	//残弾数を表示
	m_fontRender = NewGO<FontRender>(0);
	m_fontRender->SetText(L"0");
	m_fontRender->SetPivot({ 0.0f,0.0f });

	return true;
}

void AmmoGauge::Reloading(const float reloadTime)
{
	//リロード時間に応じて拡大率を更新
	m_scale.x = m_reloadTimer / reloadTime;
	//リロードの時間に達したら
	if (m_reloadTimer >= reloadTime)
	{
		//残弾数を回復
		m_remaining_ammo = m_max_ammo;
		//タイマーをリセット
		m_reloadTimer = 0.0f;
		//リロードされている
		m_isReloaded = true;
		//リロードが終わった
		m_finishReloading = true;
		return;
	}
	//リロード具合に応じて残弾の表示数を増加
	m_remaining_ammo = m_max_ammo * m_scale.x;
}

void AmmoGauge::Update()
{
	//残弾を文字にする
	std::wstring remainingAmmo = std::to_wstring(m_remaining_ammo);
	m_fontRender->SetText(remainingAmmo.c_str());

	//残弾が0になれば
	if (m_remaining_ammo == 0)
	{
		//リロードされていない状態に
		m_isReloaded = false;
	}

	//リロードされていなければ
	if (!m_isReloaded)
	{
		//ゲームタイムを用いてタイマー
		m_reloadTimer += GameTime().GetFrameDeltaTime();

		//弾の種類に応じてリロードの長さをコントロール
		switch (m_ammoType)
		{
		//マシンガンの弾の場合
		case enAmmoBullet:
			Reloading(BULLET_RELOAD_COMPLETE_TIME);
			break;
		//ミサイルの場合
		case enAmmoMissile:
			Reloading(MISSILE_RELOAD_COMPLETE_TIME);
			break;
		//レールガンの場合
		case enAmmoRaser:
			Reloading(RASER_RELOAD_COMPLETE_TIME);
			break;
		}
	}
	//リロードされているならば
	else
	{
		//リロードがそのフレームに終わったか
		m_finishReloading = false;
	}
	//残弾数に応じたゲージの長さにする
	m_gaugeScaleX = (float)m_remaining_ammo / (float)m_max_ammo;

	//文字の位置を更新
	m_fontRender->SetPosition({ -m_screenPos.x - FONT_ADJUST_AMOUNT_X, m_screenPos.y + FONT_ADJUST_AMOUNT_Y });
	//文字の拡大率を更新
	m_fontRender->SetScale(m_fontScale);
	//ゲージ画像の減少率更新
	m_spriteRender->SetSpriteSizeRate(1.0f - m_gaugeScaleX);
	//画像の位置更新
	m_spriteRender->SetPosition(m_screenPos);
}