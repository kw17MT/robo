#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"
#include "AmmoGauge.h"
#include "Enemy.h"

namespace
{
	const int MAX_AMMO = 20;				//最大数
}

MissileGenerator::~MissileGenerator()
{
	//今出ているミサイルをすべて消す
	for (auto i : m_missiles)
	{
		DeleteGO(m_missiles.back());
		m_missiles.pop_back();
	}
	m_missiles.clear();
}

void MissileGenerator::SaveTargetedEnemy(Enemy* enemy)
{
	//もっとミサイルでロックオンできるなら
	if (m_canTargetMore) {
		//ロックオンしてきた敵の数を増やす
		m_lockOnTargetNum++;
		//もう上限に達し、これ以上ロックオンできなければ
		if (m_lockOnTargetNum >= MAX_AMMO)
		{
			//できないことを保存する
			m_canTargetMore = false;
		}
		//保存用配列にインスタンスを保存
		m_enemy.push_back(enemy);
	}
}

bool MissileGenerator::Start()
{
	//残弾数ゲージを作成
	m_ammoGauge = NewGO<AmmoGauge>(0);
	//弾のタイプをミサイルに設定
	m_ammoGauge->SetAmmoType(enAmmoMissile);
	//発射可能数を教える
	m_ammoGauge->SetRemainingAmmo(m_remaining_missile);
	//所持上限数を教える
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	//位置固定
	m_ammoGauge->SetPosition({ -500.0f, -300.0f, 0.0f });

	//todo 要変更→実装方法や数値
	//ミサイルインスタンス格納配列のメモリを確保
	m_missiles.reserve(100);

	return true;
}

void MissileGenerator::Update()
{
	//初期ではアイコンを消さないように設定
	m_deleteMissileIcon = false;

	//ミサイルはリロードされているか
	if (m_ammoGauge->GetIsReloaded()) {
		//LB2ボタンを押しているならば
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//発射の準備しているよ
			m_isPrepareLaunch = true;
		}
		//発射の準備ができていて、ボタンを離したら
		if (m_isPrepareLaunch && !g_pad[0]->IsPress(enButtonLB2))
		{
			//既に発射したミサイルの数を計算
			int alreadyLaunchedNum = MAX_AMMO - m_remaining_missile;
			//今までロックオンしてきた数からすでに発射済みのミサイルの数を減算し
			//このフレームで発射すべきミサイル数を計算
			int LaunchNumInThisFrame = m_lockOnTargetNum - alreadyLaunchedNum;
			//発射数分ロケットをだす
			for (int i = 0; i < LaunchNumInThisFrame; i++)
			{
				//ミサイル生成
				m_missiles.push_back(NewGO<Missile>(0));
				//追跡する敵を教える
				m_missiles.back()->SetEnemy(m_enemy.back());
				//発射元を設定
				m_missiles.back()->SetLaunchedPos(m_launchPos);
				//使った敵のインスタンスはいらないので情報破棄
				m_enemy.pop_back();
			}
			//敵の情報を一度全部忘れる
			m_enemy.clear();

			//残り弾数から今のフレームで発射したミサイルの数を減算
			m_remaining_missile -= LaunchNumInThisFrame;
			//更新した残り弾数をゲージに教える
			m_ammoGauge->SetRemainingAmmo(m_remaining_missile);

			//発射準備の解除
			m_isPrepareLaunch = false;
			//ロックオンで使用していたアイコンを消去
			m_deleteMissileIcon = true;

			//何もロックオンしていない状態にする
			CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
		}
	}
	//残弾数がなくなったら
	if (m_remaining_missile == 0)
	{
		//もうロックオンできない
		m_canTargetMore = false;
		//飽和状態にする
		CaptureStateManager::GetInstance().SetMissileTargetState(enFull);
	}
	//ゲージでリロードが終わったら
	if (m_ammoGauge->IsReloaded())
	{
		//残弾数を最大値で初期化
		m_remaining_missile = MAX_AMMO;
		//いままでロックオンしてきた数をリセット
		m_lockOnTargetNum = 0;
		//さらにロックオンできる
		m_canTargetMore = true;
		//何もロックオンしていない状態に変えてやる
		CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
	}
}