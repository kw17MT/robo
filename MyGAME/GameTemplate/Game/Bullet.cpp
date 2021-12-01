#include "stdafx.h"
#include "Bullet.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const int BULLET_SPEED = 100.0f;
}

Bullet::~Bullet()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_effect);
}

bool Bullet::Start()
{
	//弾モデルの生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->Init("Assets/modelData/bullet/a.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);


	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/bulletTail2.efk");
	m_effect->SetScale({ 5.0f,5.0f,10.0f });

	m_effect->Update();
	return true;
}

Vector3 Bullet::CalcToTargetVec()
{
	//正規化されたターゲット地点へのベクトルを計算する
	Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Bullet::BehaveChangedByOwner()
{
	switch (m_owner)
	{
	case enPlayer:
		//更新した位置座標とすべての敵との距離を求める
		QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
			//敵と弾の距離を計算する。
			Vector3 diff = enemy->GetPosition() - m_position;
			if (diff.Length() < 200.0f) {
				if (!enemy->IsDead()) {
					//マシンガンの弾からダメージを受けたことを知らせる
					enemy->TakenDamage(enBullet);
				}
				//死亡。
				DeleteGO(this);
				//終了。
				return false;
			}
			//クエリは継続。
			return true;
			});
		break;

	case enEnemy:
		//更新した位置座標とすべての敵との距離を求める
		QueryGOs<Player>("player", [&](Player* player) {
			//敵と弾の距離を計算する。
			Vector3 diff = player->GetPosition() - m_position;
			if (diff.Length() < 200.0f) {
				//マシンガンの弾からダメージを受けたことを知らせる
				player->TakenDamage(enEnemyBullet);
				//死亡。
				DeleteGO(this);
				//終了。
				return false;
			}
			//クエリは継続。
			return true;
			});
		break;
	}
	
}

void Bullet::SetRotation(Quaternion rot)
{ 
	m_rot = rot;
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
	m_skinModelRender->SetRotation(m_rot);

	BehaveChangedByOwner();

	m_effect->SetPosition(m_position);
	m_effect->SetRotation(m_rot);
	m_effect->Play();
	//弾の寿命
	count += GameTime().GetFrameDeltaTime();
	if (count >= 3.0f)
	{
		DeleteGO(this);
	}
}