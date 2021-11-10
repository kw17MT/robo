#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"
#include "AmmoGauge.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 0.2f,0.2f,0.2f };
	const int MAX_AMMO = 100;
}

MachinGun::~MachinGun()
{
	//現在生きている弾の削除
	for (auto i : m_bullets)
	{
		DeleteGO(m_bullets.back());
		m_bullets.pop_back();
	}
	m_bullets.clear();
	//モデルの削除
	DeleteGO(m_skinModelRender);

	DeleteGO(m_ammoGauge);
}

bool MachinGun::Start()
{
	//マシンガンのモデル生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	m_ammoGauge = NewGO<AmmoGauge>(0);
	m_ammoGauge->SetAmmoType(enAmmoBullet);
	m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	m_ammoGauge->SetMaxAmmo(MAX_AMMO);
	m_ammoGauge->SetPosition({ -400.0f, -300.0f, 0.0f });

	return true;
}

void MachinGun::Update()
{
	//to do ステートメントで管理する事
	if (g_pad[0]->IsPress(enButtonRB2) && m_remaining_bullet > 0)
	{
		//弾を生成
		m_bullets.push_back(NewGO<Bullet>(0));
		//弾の初期座標系を設定
		m_bullets.back()->SetTargetAndCurrentPos(m_targetPos, m_position);
		//残り弾数を1つマイナス
		m_remaining_bullet--;
		m_ammoGauge->SetRemainingAmmo(m_remaining_bullet);
	}

	if(m_ammoGauge->IsReloaded())
	{
		m_remaining_bullet = MAX_AMMO;
	}

	//モデルの位置を更新する
	m_skinModelRender->SetPosition(m_position);
}