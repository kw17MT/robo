#include "stdafx.h"
#include "EnemyMachinGun.h"
#include "SkinModelRender.h"
#include "Bullet.h"

namespace
{
	const Vector3 MACHINGUN_SCALE = { 0.2f,0.2f,0.2f };
}

EnemyMachinGun::~EnemyMachinGun()
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
}

bool EnemyMachinGun::Start()
{
	//マシンガンのモデル生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	return true;
}

void EnemyMachinGun::Update()
{
	if (m_canShoot)
	{
		//弾を生成
		m_bullets.push_back(NewGO<Bullet>(0));
		//弾の初期座標系を設定
		m_bullets.back()->SetTargetAndCurrentPos(m_targetPos, m_position);
		m_bullets.back()->SetOwner(enEnemy);
		Quaternion rot;
		rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_targetPos - m_position);
		m_bullets.back()->SetRotation(rot);
		m_canShoot = false;
	}
	//モデルの位置を更新する
	m_skinModelRender->SetPosition(m_position);
}