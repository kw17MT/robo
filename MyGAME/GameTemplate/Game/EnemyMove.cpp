#include "stdafx.h"
#include "EnemyMove.h"

namespace
{
	const float ADJUST_SLOPE_UP_AMOUNT = 0.8f;
	const float  ADJUST_SLOPE_DOWN_AMOUNT = -ADJUST_SLOPE_UP_AMOUNT;
}

Vector3 EnemyMove::CalcNextPos(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 nextPos = enemyPos;
	//中心とするターゲットまでのベクトルを求める
	Vector3 toTargetVec = targetPos - enemyPos;
	//通常移動時は高度は下げない
	toTargetVec.y = 0.0f;
	//移動速度に利用するため正規化を行う
	toTargetVec.Normalize();

	Quaternion qRot;
	//Y軸を基軸に89度曲げる
	qRot.SetRotationDegY(89.0f);
	//減点までのベクトルを89度曲げる
	qRot.Apply(toTargetVec);
	//今のフレームの移動方向を記録
	m_prevMoveDirection = toTargetVec;
	//プレイヤーを囲むモードだったら
	if (m_moveType == enSurroundPlayer)
	{
		//いつもよりゆっくりプレイヤー周りを旋回
		nextPos += toTargetVec * m_moveSpeed / 3.0f;
	}
	else
	{
		//89度曲げたベクトルに速度を適用して次の位置座標を計算する。
		nextPos += toTargetVec * m_moveSpeed;
	}

	return nextPos;
}

Vector3 EnemyMove::CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos)
{
	//ターゲットとなるプレイヤーへのベクトルとその正規化
	Vector3 toTargetVec = targetPos - enemyPos;
	//プレイヤーに近づきすぎたら
	if (toTargetVec.Length() < 1000.0f)
	{
		//逃げる処理にするかどうか
		srand((unsigned int)time(NULL));
		bool isRun = rand() % 2;
		if (isRun) {
			//逃げに転換
			m_moveType = enRun;
		}
		else
		{
			//プレイヤーを囲んで積極的に攻撃
			m_moveType = enSurroundPlayer;
		}
	}
	toTargetVec.Normalize();

	//ロックオンしていた敵が倒されたら
	//進行していた方向＝現在のミサイルの前方向とターゲットへのベクトル内積を求める。
	if (float dot = toTargetVec.Dot(m_prevMoveDirection) > 0.99f)
	{
		//移動方向を保存
		m_prevMoveDirection = toTargetVec;
		//そのまま計算した速度を適用＝このままでも逃げになるため
		Vector3 nextPos = enemyPos + toTargetVec * m_moveSpeed;

		return nextPos;
	}
	else
	{
		
		//そのまま計算した速度を適用＝このままでも逃げになるため
		Vector3 nextPos = enemyPos + toTargetVec * m_moveSpeed;

		
		//ターゲットへのベクトルとミサイルの前方向の上方向を計算
		Vector3 up = toTargetVec.CalcCross(m_prevMoveDirection);
		Quaternion rot;
		rot.SetRotationDeg(up, dot * 100.0f);
		rot.Apply(nextPos);

		//移動方向を保存
		m_prevMoveDirection = toTargetVec;

		return nextPos;
	}
}

Vector3 EnemyMove::CalcRunPos(Vector3 enemyPos, Vector3 targetPos)
{
	//逃げるモードになる前の移動方向を用いて素通りするように動かす。
	return enemyPos + m_prevMoveDirection * m_moveSpeed;
}

Vector3 EnemyMove::AdjustedAltitudePos(Vector3 calcedPos)
{
	switch (m_altitudeState)
	{
	//ワールドに対して高度が高すぎているならば
	case enTooHigh:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_DOWN_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		calcedPos.y += m_adjustedAltitudeHeight;
		if (calcedPos.y <= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return calcedPos;
		break;
	//ワールドに対して高度が低すぎている場合
	case enTooLow:
		m_adjustingAltitudeElapsedTime += GameTime().GetFrameDeltaTime();
		m_adjustedAltitudeHeight = ADJUST_SLOPE_UP_AMOUNT * (m_adjustingAltitudeElapsedTime * m_adjustingAltitudeElapsedTime);
		calcedPos.y += m_adjustedAltitudeHeight;
		if (calcedPos.y >= 9000.0f)
		{
			m_altitudeState = enSafe;
			m_prevMoveDirection.y = 0.0f;
		}
		return calcedPos;
		break;
	//正常な場合
	default:
		m_adjustingAltitudeElapsedTime = 0.0f;
		m_adjustedAltitudeHeight /= 1.1f;
		if (abs(m_adjustedAltitudeHeight) < 10.0f)
		{
			m_adjustedAltitudeHeight = 0.0f;
		}
		calcedPos.y += m_adjustedAltitudeHeight;
		return calcedPos;
		break;
	}
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	switch (m_moveType)
	{
	case enNormalMove:
		return AdjustedAltitudePos(CalcNextPos(enemyPos));
		break;
	case enApproach:
		return AdjustedAltitudePos(CalcApproachedPos(enemyPos, targetPos));
		break;
	case enRun:
		return AdjustedAltitudePos(CalcRunPos(enemyPos, targetPos));
		break;
	case enAroundPlayer:
		return AdjustedAltitudePos(CalcNextPos(enemyPos, targetPos));
		break;
	case enSurroundPlayer:
		return AdjustedAltitudePos(CalcNextPos(enemyPos, targetPos));
	default:
		return AdjustedAltitudePos(CalcNextPos(enemyPos));
		break;
	}
}