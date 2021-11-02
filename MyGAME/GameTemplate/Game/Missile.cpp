#include "stdafx.h"
#include "Missile.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "CaptureStateManager.h"

namespace
{
	const int MISSILE_SPEED = 150.0f;
}

Missile::~Missile()
{
	//出現しているロケットの総数をデクリメント
	CaptureStateManager::GetInstance().MinusRockeTargetNum();

	DeleteGO(m_skinModelRender);
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/missile/missile.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_skinModelRender->SetScale({ 10.0f,10.0f,10.0f });

	return true;
}

Vector3 Missile::CalcToTargetVec()
{
	//正規化されたターゲット地点へのベクトルを計算する
	Vector3 toTargetVec = m_enemy->GetPosition() - m_position;
	//Vector3 toTargetVec = m_targetPos - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Missile::Update()
{
	//m_targetPos = CaptureStateManager::GetInstance().GetRocketTargetEnemyPos(m_number);
	//敵までの距離から移動方向と速度を計算する。
	float a;
	m_moveDirection = CalcToTargetVec();

	//ロックオンしていた敵が倒されたら
	if(m_enemy->IsDead())
	{
		//倒される前の敵への方向を使う
		m_moveDirection = m_prevMoveDirection;
	}
	else
	{
		//進行していた方向＝現在のミサイルの前方向とターゲットへのベクトル内積を求める。
		if (a = m_moveDirection.Dot(m_prevMoveDirection) > 0.8f)
		{

		}
		else
		{
			//ターゲットへのベクトルとミサイルの前方向の上方向を計算
			Vector3 up = m_moveDirection.CalcCross(m_prevMoveDirection);
			Quaternion rot;
			rot.SetRotationDeg(up, 80.0f);
			rot.Apply(m_moveDirection);
		}

		m_prevMoveDirection = m_moveDirection;
	}

	m_moveSpeed = m_moveDirection * MISSILE_SPEED;
	
	//速度を考慮した位置座標を設定する。
	m_position += m_moveSpeed;
	m_skinModelRender->SetPosition(m_position);

	//更新した位置座標とすべての敵との距離を求める
	QueryGOs<Enemy>("enemy", [&](Enemy* enemy) {
		//敵と弾の距離を計算する。
		Vector3 diff = enemy->GetPosition() - m_position;
		if (diff.Length() < 200.0f) {
			//ロケットからダメージを受けたことを知らせる
			enemy->TakenDamage(enMissile);
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
	if (count >= 30.0f)
	{
		DeleteGO(this);
	}



	m_rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_moveDirection);
	m_skinModelRender->SetRotation(m_rot);
}