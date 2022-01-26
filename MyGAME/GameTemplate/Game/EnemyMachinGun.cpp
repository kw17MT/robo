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
	//モデルの削除
	DeleteGO(m_skinModelRender);
}

bool EnemyMachinGun::Start()
{
	//マシンガンのモデル生成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/bullet/bullet.tkm", nullptr, enModelUpAxisY, true);
	m_skinModelRender->SetScale(MACHINGUN_SCALE);

	return true;
}

void EnemyMachinGun::Update()
{
	if (m_canShoot)
	{
		//弾を生成（時間で自分で消える）
		Bullet* bullet = NewGO<Bullet>(0);
		//ターゲットの位置と自分の位置を与える
		bullet->SetTargetAndCurrentPos(m_targetPos, m_position);
		//弾を発射したのは誰かを記録する
		bullet->SetOwner(enEnemy);
		Quaternion rot;
		//モデルが初期で向いている方向からターゲットへの方向へ回転させる
		rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_targetPos - m_position);
		bullet->SetRotation(rot);
		//連続で撃てないようにする
		m_canShoot = false;
	}
	//モデルの位置を更新する
	m_skinModelRender->SetPosition(m_position);
}