#include "stdafx.h"
#include "Bullet.h"
#include "SkinModelRender.h"
#include "Enemy.h"

namespace
{
	const int BULLET_SPEED = 1000.0f;
}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
}

bool Bullet::Start()
{
	//弾モデルの生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);

	return true;
}

Vector3 Bullet::CalcToTargetVec()
{
	//正規化されたターゲット地点へのベクトルを計算する
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Bullet::Update()
{
	//敵のまでの距離から移動方向と速度を計算する。
	if (m_firstCalc == false)
	{
		m_moveSpeed = CalcToTargetVec() * BULLET_SPEED;
		m_firstCalc = true;
	}

	//速度を考慮した位置座標を設定する。
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