#include "stdafx.h"
#include "EnemyMove.h"

namespace
{
	const float MOVE_SPEED = 30.0f;
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
	//89度曲げたベクトルに速度を適用して次の位置座標を計算する。
	nextPos += toTargetVec * MOVE_SPEED;

	return nextPos;
}

Vector3 EnemyMove::CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos)
{
	//ターゲットとなるプレイヤーへのベクトルとその正規化
	Vector3 toTargetVec = targetPos - enemyPos;
	//プレイヤーに近づきすぎたら
	if (toTargetVec.Length() < 500.0f)
	{
		//逃げに転換
		m_moveType = enRun;
	}
	toTargetVec.Normalize();

	/////////////////////////////////////////////////////////////////////////
	//ロックオンしていた敵が倒されたら
	// 
		//進行していた方向＝現在のミサイルの前方向とターゲットへのベクトル内積を求める。
	if (float dot = toTargetVec.Dot(m_prevMoveDirection) > 0.99f)
	{
		//移動方向を保存
		m_prevMoveDirection = toTargetVec;
		//そのまま計算した速度を適用＝このままでも逃げになるため
		Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;

		return nextPos;
	}
	else
	{
		
		//そのまま計算した速度を適用＝このままでも逃げになるため
		Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;

		
		//ターゲットへのベクトルとミサイルの前方向の上方向を計算
		Vector3 up = toTargetVec.CalcCross(m_prevMoveDirection);
		Quaternion rot;
		rot.SetRotationDeg(up, dot * 100.0f);
		rot.Apply(nextPos);

		//移動方向を保存
		m_prevMoveDirection = toTargetVec;

		return nextPos;
	}
	


	/////////////////////////////////////////////////////////////////////////



	////移動方向を保存
	//m_prevMoveDirection = toTargetVec;
	////そのまま計算した速度を適用＝このままでも逃げになるため
	//Vector3 nextPos = enemyPos + toTargetVec * MOVE_SPEED;
	//
	//return nextPos;
}

Vector3 EnemyMove::CalcRunPos(Vector3 enemyPos, Vector3 targetPos)
{
	//ターゲットとなるプレイヤーへのベクトルとその正規化
	//Vector3 toTargetVec = targetPos - enemyPos;
	//toTargetVec.Normalize();

	//逃げるための上方向回転軸を計算
	//Vector3 up;
	//up = toTargetVec.CalcCross(m_prevMoveDirection);

	//移動方向を保存
	//m_prevMoveDirection = toTargetVec;

	//Quaternion qRot;
	//180度回転させて逃げる方向を作成する。
	//qRot.SetRotation(toTargetVec, 180.0f);
	//qRot.Apply(toTargetVec);

	//逃げるモードになる前の移動方向を用いて素通りするように動かす。
	return enemyPos + m_prevMoveDirection/*toTargetVec*/ * MOVE_SPEED;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	Vector3 nextPos;
	switch (m_moveType)
	{
	case enNormalMove:
		return CalcNextPos(enemyPos);
		break;
	case enApproach:
		return CalcApproachedPos(enemyPos, targetPos);
		break;
	case enRun:
		return CalcRunPos(enemyPos, targetPos);
		break;
	case enAroundPlayer:
		return CalcNextPos(enemyPos, targetPos);
		break;
	default:
		return CalcNextPos(enemyPos);
		break;
	}
}