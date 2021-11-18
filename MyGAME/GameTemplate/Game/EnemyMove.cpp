#include "stdafx.h"
#include "EnemyMove.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float ADJUST_SLOPE_UP_AMOUNT = 0.8f;
	const float  ADJUST_SLOPE_DOWN_AMOUNT = -ADJUST_SLOPE_UP_AMOUNT;
}

void EnemyMove::JudgeMoveType(Vector3 enemyPos, Vector3 targetPos)
{
	float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
	//プレイヤーに近づきすぎたら
	if (distance < 2000.0f)
	{
		//逃げる処理にするかどうか
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % 6;

		//nextMoveState = 9;

		// OK
		if (nextMoveState == 0) {
			m_isCalcEnemyDashDirection = false;
			m_moveType = enFrontAndBehind;
		}
		// OK
		else if (nextMoveState == 1)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enPlayerBehind;
		}
		// NG
		else if (nextMoveState == 3)
		{
			m_moveType = enAround;
		}
		// OK
		else if (nextMoveState == 4)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enApproachAndDash;
		}
		else // nextMoveState == 5
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enStay;
		}
	}
}

void EnemyMove::CalcApproachSpeed(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 toTargetVec = targetPos - enemyPos;

	toTargetVec.Normalize();
	m_currentMoveDirection = toTargetVec;
	Vector3 speed = toTargetVec * m_moveSpeed;

	JudgeMoveType(enemyPos, targetPos);
}

Vector3 EnemyMove::AdjustedAltitudeSpeed(Vector3 speed)
{
	switch (m_altitudeState)
	{
	//ワールドに対して高度が高すぎているならば
	case enTooHigh:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_DOWN_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		speed.y += m_adjustedAltitudeHeight;
		if (speed.y <= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return speed;
		break;
	//ワールドに対して高度が低すぎている場合
	case enTooLow:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_UP_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		speed.y += m_adjustedAltitudeHeight;
		if (speed.y >= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return speed;
		break;
	//正常な場合
	default:
		m_adjustingAltitudeElapsedTime = 0.0f;
		m_adjustedAltitudeHeight /= 1.1f;
		if (abs(m_adjustedAltitudeHeight) < 10.0f)
		{
			m_adjustedAltitudeHeight = 0.0f;
		}
		speed.y += m_adjustedAltitudeHeight;
		return speed;
		break;
	}
}

void EnemyMove::CalcFrontAndBehindSpeed(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 enemyToTargetVec = targetPos - enemyPos;
	enemyToTargetVec.Normalize();
	//自分はプレイヤーの後ろにいるのか前にいるのか
	float dot = enemyToTargetVec.Dot(g_camera3D->GetForward());
	if (dot < 0.2f && m_moveType == enPlayerBehind)
	{
		m_moveType = enDash;
	}
	else
	{
		float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
		if (distance > 7000.0f)
		{
			m_moveForward = true;

			bool isRound = rand() % 2;
			if (isRound)
			{
				m_moveType = enAround;
			}
		}
		if (distance < 2000.0f)
		{
			m_moveForward = false;
		}

		if (m_moveForward)
		{
			m_currentMoveDirection = enemyToTargetVec;
		}
		else
		{
			m_currentMoveDirection = enemyToTargetVec * -1.0f;
		}
	}
}

void EnemyMove::CalcAroundSpeed(Vector3 enemyPos, Vector3 targetPos)
{
	m_aroundTimer += GameTime().GetFrameDeltaTime();
	if (m_aroundTimer >= 3.0f)
	{
		m_aroundTimer = 0.0f;
		m_moveType = enApproach;
	}

	//中心とするターゲットまでのベクトルを求める
	Vector3 toTargetVec = targetPos - enemyPos;
	//移動速度に利用するため正規化を行う
	toTargetVec.Normalize();

	Vector3 sideVec = toTargetVec.CalcCross({ 0.0f,1.0f,0.0f });
	sideVec.y += toTargetVec.y * 5.0f;

	toTargetVec += sideVec;
	toTargetVec *= -1.0f;

	//今のフレームの移動方向を記録
	m_currentMoveDirection = toTargetVec;
}

void EnemyMove::CalcApproachAndDashSpeed(Vector3 enemyPos, Vector3 targetPos)
{
	//中心とするターゲットまでのベクトルを求める
	Vector3 toTargetVec = targetPos - enemyPos;
	//移動速度に利用するため正規化を行う
	toTargetVec.Normalize();

	float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
	if (distance < 2000.0f)
	{
		//ダッシュでプレイヤーを避けるようにどこかへ
		EnemyDashSpeed(enemyPos, targetPos, 5.0f);
	}
	else
	{
		m_currentMoveDirection = toTargetVec;
	}

	if (distance >= 7000.0f)
	{
		m_moveType = enApproach;
	}
}

void EnemyMove::EnemyDashSpeed(Vector3 enemyPos, Vector3 targetPos, float dashTime)
{
	if (!m_isCalcEnemyDashDirection)
	{
		//中心とするターゲットまでのベクトルを求める
		Vector3 toTargetVec = targetPos - enemyPos;
		//移動速度に利用するため正規化を行う
		toTargetVec.Normalize();

		Vector3 axisSide = toTargetVec.CalcCross({ 0.0f,1.0f,0.0f });
		Vector3 axisUp = toTargetVec.CalcCross(axisSide);

		float degreeX = rand() % 90 + 1;
		float degreeY = rand() % 90 + 1;
		degreeX -= 45.0f; degreeY -= 45.0f;
		if (degreeX > -30.0f && degreeX < 0.0f)
		{
			degreeX = -30.0f;
		}
		else if(degreeX > 0.0f && degreeX < 30.0f)
		{
			degreeX = 30.0f;
		}

		if (degreeY > -30.0f && degreeY < 0.0f)
		{
			degreeY = -30.0f;
		}
		else if (degreeY > 0.0f && degreeY < 30.0f)
		{
			degreeY = 30.0f;
		}
		Quaternion qRot;
		qRot.SetRotationDeg(axisSide, degreeY);
		qRot.Apply(toTargetVec);
		qRot.SetRotationDeg(axisUp, degreeX);
		qRot.Apply(toTargetVec);

		m_currentMoveDirection = toTargetVec;
		m_isCalcEnemyDashDirection = true;
	}

	m_dashTimer += GameTime().GetFrameDeltaTime();

	if (m_dashTimer >= 5.0f)
	{
		m_moveType = enApproach;

		m_isCalcEnemyDashDirection = false;
		m_dashTimer = 0.0f;
		m_waitTimer = 0.0f;
	}
}

void EnemyMove::EnemyStaySpeed(Vector3 enemyPos, Vector3 targetPos)
{
	m_waitTimer += GameTime().GetFrameDeltaTime();

	if (m_waitTimer >= 10.0f)
	{
		return EnemyDashSpeed(enemyPos, targetPos);
	}

	//中心とするターゲットまでのベクトルを求める
	Vector3 toTargetVec = targetPos - enemyPos;
	//移動速度に利用するため正規化を行う
	toTargetVec.Normalize();

	Vector3 sideVec = toTargetVec.CalcCross({ 0.0f,1.0f,0.0f });

	m_currentMoveDirection = sideVec/*m_prevMoveDirection*/ / 10.0f;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
	if (distance > 7100.0f)
	{
		m_moveType = enApproach;
	}

	if (distance < 500.0f)
	{
		m_moveForward = false;
		m_moveType = enFrontAndBehind;
	}

	switch (m_moveType)
	{
	case enApproach:
		CalcApproachSpeed(enemyPos, targetPos);
		break;
	case enFrontAndBehind:
		CalcFrontAndBehindSpeed(enemyPos, targetPos);
		break;
	case enAround:
		CalcAroundSpeed(enemyPos, targetPos);
		break;
	case enApproachAndDash:
		CalcApproachAndDashSpeed(enemyPos, targetPos);
		break;
	case enPlayerBehind:
		CalcFrontAndBehindSpeed(enemyPos, targetPos);
		break;
	case enStay:
		EnemyStaySpeed(enemyPos, targetPos);
		break;
	case enDash:
		EnemyDashSpeed(enemyPos, targetPos);
		break;
	default:
		CalcApproachSpeed(enemyPos, targetPos);
		break;
	}

	if (m_prevMoveType != m_moveType)
	{
		m_prevMoveDirection = m_currentMoveDirection;
		m_prevMoveType = m_moveType;
		m_acceralation = 0.7f;
	}

	m_acceralation -= (GameTime().GetFrameDeltaTime() / 10.0f);
	if (m_acceralation <= 0.0f)
	{
		m_acceralation = 0.0f;
	}

	Vector3 finalSpeed;
	finalSpeed = 
		(m_prevMoveDirection * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation))
		* m_moveSpeed;

	return enemyPos + finalSpeed;
}