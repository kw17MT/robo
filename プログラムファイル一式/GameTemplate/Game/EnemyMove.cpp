#include "stdafx.h"
#include "EnemyMove.h"

#include "EnemyApproachState.h"
#include "EnemyDashState.h"
#include "EnemyAroundState.h"
#include "EnemyStayState.h"
#include "EnemyBehindState.h"
#include "EnemyBackState.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float ADJUST_ACCERALATION_RATE = 5.0f;						//加速度の増加調整用
	const int MAX_ROTATION_DEGREE = 60;									//回転する最大量
	const float SPEED_DECAY_RATE = 10.0f;								//速度の減少率
}

void EnemyMove::CalcApproachDirection(Vector3 toTargetVec)
{
	//プレイヤーに向かう移動方向
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);
}

void EnemyMove::CalcBackDirection(Vector3 toTargetVec)
{
	//プレイヤーと反対方向に向かうベクトル
	m_currentMoveDirection = (toTargetVec * -1.0f) * m_acceralation + (toTargetVec * (1.0f - m_acceralation));
}

void EnemyMove::CalcAroundDirection(Vector3 toTargetVec)
{
	//プレイヤーに対する横方向を算出
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);

	//後退しながら横に移動する正規化移動方向を計算する。
	toTargetVec += sideVec;
	toTargetVec *= -1.0f;
	toTargetVec.Normalize();

	//加速度に応じて適用移動方向を決める
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);;
}

void EnemyMove::CalcDashDirection(Vector3 toTargetVec)
{
	//最初のダッシュ方向の計算が終わっていなければ
	if (!m_isCalcEnemyDashDirection)
	{
		//横方向ベクトルの計算
		Vector3 axisSide = toTargetVec.CalcCross(g_vec3AxisY);
		//上方向ベクトルの計算
		Vector3 axisUp = toTargetVec.CalcCross(axisSide);

		//1~60の数値を得る
		float degreeZ = rand() % MAX_ROTATION_DEGREE + 1;
		float degreeY = rand() % MAX_ROTATION_DEGREE + 1;
		//数値を-29~30に調節する
		degreeZ -= MAX_ROTATION_DEGREE / 2.0f;
		degreeY -= MAX_ROTATION_DEGREE / 2.0f;

		Quaternion qRot;
		//Y方向への回転を計算、移動ベクトルに適用する。
		qRot.SetRotationDeg(axisSide, degreeY);
		qRot.Apply(toTargetVec);
		//方向への回転を計算、移動ベクトルに適用する。
		qRot.SetRotationDeg(axisUp, degreeZ);
		qRot.Apply(toTargetVec);

		m_dashDirection = toTargetVec;
		m_isCalcEnemyDashDirection = true;
	}
	//前の移動速度を加味したダッシュ方向を計算
	m_currentMoveDirection = m_dashDirection * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);
}

void EnemyMove::CalcStayDirection(Vector3 toTargetVec)
{
	//敵の横方向を計算する。
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);
	//前の移動方向を加味した漂う方向を計算する。
	m_currentMoveDirection = m_currentMoveDirection * (1.0f - m_acceralation) + sideVec * m_acceralation / SPEED_DECAY_RATE;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	//敵からプレイヤーへの方向を計算する。
	Vector3 toTargetVec = targetPos - enemyPos;
	toTargetVec.Normalize();

	//プレイヤーと敵との距離を計算する。
	m_param.distanceBetweenEnemyToPlayer = CalcMethods::CalcDistance(enemyPos, targetPos);;
	m_param.toTargetDirection = toTargetVec;

	//現在の移動状態を時間やプレイヤーとの距離に応じて更新する。
	EnemyMoveState* currentState = m_state->Update(m_param);
	//更新予定の状態と現在の状態が違っていたら
	if (currentState != m_state)
	{
		//状態を更新
		m_state = currentState;
		//タイマーなどのパラメータを初期化
		m_state->Init(m_param);
		//加速を無くす
		m_acceralation = 0.0f;
		//ダッシュ方向を計算しなおす
		m_isCalcEnemyDashDirection = false;
	}

	if (m_state == EnemyApproachState::GetInstance())
	{
		CalcApproachDirection(toTargetVec);
	}
	else if(m_state == EnemyDashState::GetInstance())
	{
		CalcDashDirection(toTargetVec);
	}
	else if (m_state == EnemyAroundState::GetInstance())
	{
		CalcAroundDirection(toTargetVec);
	}
	else if (m_state == EnemyBehindState::GetInstance())
	{
		CalcDashDirection(toTargetVec);
	}
	else if (m_state == EnemyBackState::GetInstance())
	{
		CalcBackDirection(toTargetVec);
	}
	else
	{
		CalcStayDirection(toTargetVec);
	}

	//時間で加速度を上げていく
	m_acceralation += (GameTime().GetFrameDeltaTime() / ADJUST_ACCERALATION_RATE);
	//1以上にはならないように
	if (m_acceralation >= 1.0f)
	{
		m_acceralation = 1.0f;
	}

	//移動方向と自分のスピードを現在の位置に加算し、新しい位置座標を返す。
	return enemyPos + m_currentMoveDirection * m_moveSpeed;
}