#include "stdafx.h"
#include "MachinGun.h"
#include "SkinModelRender.h"
#include "Enemy.h"

namespace
{
	const float BULLET_SPEED = 1000.0f;
}

MachinGun::~MachinGun()
{
	DeleteGO(m_skinModelRender);
	m_skinModelRender = nullptr;
}

bool MachinGun::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);

	return true;
}

Vector3 MachinGun::CalcToTargetVec()
{
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void MachinGun::Update()
{
	//敵のまでの距離から移動方向と速度を計算する。
	if (m_firstCalc == false)
	{
		m_moveSpeed = CalcToTargetVec() * BULLET_SPEED;
		m_firstCalc = true;
	}

	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);

	//更新した位置座標とすべての敵との距離を求める
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//敵と弾の距離を計算する。
		Vector3 diff = enemy->GetPosition() - m_position;
		if (diff.Length() < 200.0f) { 
			//マシンガンの弾からダメージを受けたことを知らせる
			enemy->TakenDamage(enBullet);
			//死亡。
			DeleteGO(this);
			//終了。
			return false;
		}
		//クエリは継続。
		return true;
		});

	//弾の寿命
	count += GameTime().GetFrameDeltaTime();
	if (count >= 3.0f)
	{
		DeleteGO(this);
	}
}