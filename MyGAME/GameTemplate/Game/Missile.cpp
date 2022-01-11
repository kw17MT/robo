#include "stdafx.h"
#include "Missile.h"
#include "SkinModelRender.h"
#include "Enemy.h"
#include "CaptureStateManager.h"
#include "effect/Effect.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace
{
	const float MAX_MISSILE_SPEED = 150.0f;
	const float DEPLOY_SPEED = 40.0f;
}

Missile::~Missile()
{
	DeleteGO(m_skinModelRender);
	Effect effect;
	effect.Init(u"Assets/effect/aa.efk");
	m_rot.SetRotation({ 0.0f,0.0f,1.0f }, m_moveDirection);
	effect.SetPosition(m_position);
	effect.SetRotation(m_rot);
	effect.SetScale({ 400.0f,400.0f,400.0f });
	effect.Play();
	effect.Update();
}

bool Missile::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/missile/missile.tkm", nullptr, enModelUpAxisZ, true);
	m_skinModelRender->SetScale({ 15.0f,15.0f,15.0f });

	m_deployDirection = CalcDeployDirection();
	m_prevMoveDirection = m_deployDirection;

	m_currentMoveSpeedRate = DEPLOY_SPEED / MAX_MISSILE_SPEED;

	return true;
}

Vector3 Missile::CalcDeployDirection()
{
	//画面の前方向を取得
	Vector3 front = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	Vector3 up = front.CalcCross(right);

	//srand((unsigned int)time(NULL));				//ミサイル自体は同じフレームで一気に出しているため、現在時刻で初期化しないこと
	float degreeX = rand() % 90 + 1;
	float degreeY = rand() % 90 + 1;
	degreeX -= 45.0f; degreeY -= 45.0f;
	Quaternion qRot;
	qRot.SetRotationDeg(right, degreeY);
	qRot.Apply(front);
	qRot.SetRotationDeg(up, degreeX);
	qRot.Apply(front);

	return front;
}

Vector3 Missile::CalcToTargetVec()
{
	//正規化されたターゲット地点へのベクトルを計算する
	Vector3 toTargetVec = m_enemy->GetPosition() - m_position;
	toTargetVec.Normalize();
	return toTargetVec;
}

void Missile::RestrictRotation()
{
	//ロックオンしていた敵が倒されたら
	if (m_enemy->IsDead())
	{
		////倒される前の敵への方向を使う
		//m_moveDirection = m_prevMoveDirection;
	}
	else
	{
		//進行していた方向＝現在のミサイルの前方向とターゲットへのベクトル内積を求める。
		if (float a = m_moveDirection.Dot(m_prevMoveDirection) > 0.8f)
		{
			a;
		}
		else
		{
			//ターゲットへのベクトルとミサイルの前方向の上方向を計算
			Vector3 up = m_moveDirection.CalcCross(m_prevMoveDirection);
			Quaternion rot;
			rot.SetRotationDeg(up, 10.0f);
			rot.Apply(m_moveDirection);
		}
		m_prevMoveDirection = m_moveDirection;
	}
}

void Missile::Update()
{
	if (m_enemy->IsDead())
	{
		m_moveStage = enPassingBy;
	}

	switch (m_moveStage)
	{
	//ロケット発射直後のランダム方向への展開
	case enDeploying:
		//最初に計算した展開方向を使用してミサイルを展開
		m_moveSpeed = m_deployDirection * DEPLOY_SPEED;
		//1秒くらいたったら
		if (count >= 1.0f)
		{
			//直進モードに切り替え
			m_moveStage = enChaseTarget;
		}
		//1フレーム前の移動方向として保存
		m_prevMoveDirection = m_deployDirection;
		break;

	//敵を寿命が尽きるまで追跡する
	case enChaseTarget:
		//敵までの距離から移動方向と速度を計算する。
		m_moveDirection = CalcToTargetVec();

		if (m_enemy->IsDead())
		{
			m_moveSpeed = m_prevMoveDirection * MAX_MISSILE_SPEED;
			break;
		}

		//抑制済みの移動方向を用いて速度を計算
		m_currentMoveSpeedRate += 0.01f;
		if (m_currentMoveSpeedRate >= 1.0f)
		{	
			m_currentMoveSpeedRate = 1.0f;
		}

		//展開方向の影響を受けた移動速度
		{
			Vector3 directionAffectedDeploy = m_deployDirection * (1.0f - m_currentMoveSpeedRate);
			Vector3 speedAffectedDeploy = directionAffectedDeploy * DEPLOY_SPEED;
			//ターゲットへの方向を考慮した移動速度
			Vector3 directionFallowDirection = m_moveDirection * m_currentMoveSpeedRate;
			Vector3 speedFallowDirection = directionFallowDirection * MAX_MISSILE_SPEED;


			m_moveDirection = directionAffectedDeploy + directionFallowDirection;


			//回転の抑制
			RestrictRotation();
			m_prevMoveDirection = m_moveDirection;

			//現在の移動速度
			m_moveSpeed = speedAffectedDeploy + speedFallowDirection;
		}
		break;

	case enPassingBy:
		m_moveSpeed = m_prevMoveDirection * MAX_MISSILE_SPEED;
		break;
	}

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
	if (count >= 15.0f)
	{
		DeleteGO(this);
	}

	//現在の移動方向を用いてミサイルモデルの回転を行う
	m_rot.SetRotation({ 0.0f,0.0f,-1.0f }, m_moveDirection);
	m_skinModelRender->SetRotation(m_rot);

	if (m_moveStage != enDeploying)
	{
		Effect effect;
		effect.Init(u"Assets/effect/aa.efk");
		m_rot.SetRotation({ 0.0f,0.0f,1.0f }, m_moveDirection);
		effect.SetPosition(m_position);
		effect.SetRotation(m_rot);
		effect.SetScale({ 20.0f,20.0f,120.0f });
		effect.Play();
		effect.Update();
	}
}