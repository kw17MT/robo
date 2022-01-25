#include "stdafx.h"
#include "EnemyMove.h"

extern float CalcMethods::CalcDistance(Vector3 v1, Vector3 v2);

namespace
{
	const float DOT_ENEMY_RECOGNIZE_PLAYER_FRONT = -0.85f;				//敵がプレイヤーのどのくらい正面にいるか（内積結果）
	const float MAX_WAIT_TIME = 10.0f;									//Stayモードでの待機する最大時間
	const float MAX_AROUND_TIME = 6.0f;									//旋回する最大時間
	const float MAX_DASH_TIME = 5.0f;									//ダッシュする最大時間
	const float ADJUST_ACCERALATION_RATE = 5.0f;						//加速度の増加調整用
	const float LIMIT_ENEMY_CAN_APPROACH = 2000.0f;						//敵が近づこうとする限度
	const float LIMIT_ENEMY_RECOGNIZE_PLAYER = 7000.0f;					//敵がプレイヤーを認識できる限界距離
	const int MAX_ROTATION_DEGREE = 60;
	const int MOVE_STATE_NUM = 5;
	const float SPEED_DECAY_RATE = 10.0f;
}

void EnemyMove::JudgeMoveType(float distanceBetweenEnemyToPlayer)
{
	//プレイヤーに近づきすぎたら
	if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
	{
		//移動タイプをランダムに選択
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % MOVE_STATE_NUM;

		//敵を向いて前後に動く
		if (nextMoveState == enFrontAndBehind) {
			m_isCalcEnemyDashDirection = false;
			m_moveType = enFrontAndBehind;
		}
		//プレイヤーの後ろをとった後前後に動く
		else if (nextMoveState == enPlayerBehind)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enPlayerBehind;
		}
		//プレイヤー中心に旋回する
		else if (nextMoveState == enAround)
		{
			m_moveType = enAround;
		}
		//近づいてダッシュで通り抜ける
		else if (nextMoveState == enApproachAndDash)
		{
			m_isCalcEnemyDashDirection = false;
			m_moveType = enApproachAndDash;
		}
		//プレイヤーの近くを漂う
		else // nextMoveState == 4
		{
			m_moveType = enStay;
		}
	}
}

void EnemyMove::CalcApproachSpeed(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	//前フレームの移動方向を加味したプレイヤーに向かう移動方向
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);
	//近づいた結果、移動タイプを変えるべきかどうかの判断
	JudgeMoveType(distanceBetweenEnemyToPlayer);
}

void EnemyMove::CalcFrontAndBehindDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	//自分はプレイヤーの後ろにいるのか前にいるのか
	float dot = toTargetVec.Dot(g_camera3D->GetForward());
	//プレイヤーの背後をとるモードなのに、プレイヤーの正面付近に来ていたら
	if (dot < DOT_ENEMY_RECOGNIZE_PLAYER_FRONT && m_moveType == enPlayerBehind)
	{
		//ダッシュで方向転換する
		m_moveType = enDash;
	}
	else
	{
		//規定以上なら
		if (distanceBetweenEnemyToPlayer > LIMIT_ENEMY_RECOGNIZE_PLAYER)
		{
			//前フレームまでは後退モードなら
			if (!m_moveForward)
			{
				//加速度をリセットする。
				m_acceralation = 0.0f;
			}
			//前進モードにする
			m_moveForward = true;

			//半分の確率でプレイヤー中心に回るモードにする
			bool isRound = rand() % 2;
			if (isRound)
			{
				m_moveType = enAround;
			}
		}
		//規定の距離未満になれば
		else if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
		{
			//前フレームまでは前進モードだった場合
			if (m_moveForward)
			{
				//加速度をリセット
				m_acceralation = 0.0f;
			}
			//後退モードにする
			m_moveForward = false;
		}

		//前進モードだった場合の速度計算
		if (m_moveForward)
		{
			m_currentMoveDirection = (toTargetVec * m_acceralation) + ((toTargetVec * -1.0f) * (1.0f - m_acceralation));
		}
		//後退モードだった場合の速度計算
		else
		{
			m_currentMoveDirection = (toTargetVec * -1.0f) * m_acceralation + (toTargetVec * (1.0f - m_acceralation));
		}
	}
}

void EnemyMove::CalcAroundDirection(Vector3 toTargetVec)
{
	m_aroundTimer += GameTime().GetFrameDeltaTime();
	if (m_aroundTimer >= MAX_AROUND_TIME)
	{
		m_aroundTimer = 0.0f;
		m_moveType = enApproach;
	}

	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);

	toTargetVec += sideVec;
	toTargetVec *= -1.0f;
	toTargetVec.Normalize();

	//今のフレームの移動方向を記録
	m_currentMoveDirection = toTargetVec * m_acceralation + m_currentMoveDirection * (1.0f - m_acceralation);;
}

void EnemyMove::CalcApproachAndDashDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer)
{
	if (distanceBetweenEnemyToPlayer < LIMIT_ENEMY_CAN_APPROACH)
	{
		//ダッシュでプレイヤーを避けるようにどこかへ
		m_moveType = enDash;
	}
	else
	{
		m_currentMoveDirection = toTargetVec;
	}

	if (distanceBetweenEnemyToPlayer >= LIMIT_ENEMY_RECOGNIZE_PLAYER)
	{
		m_moveType = enApproach;
	}
}

void EnemyMove::EnemyDashDirection(Vector3 toTargetVec)
{
	//最初のダッシュ方向の計算が終わっていなければ
	if (!m_isCalcEnemyDashDirection)
	{
		//横方向ベクトルの計算
		Vector3 axisSide = toTargetVec.CalcCross(g_vec3AxisY);
		//上方向ベクトルの計算
		Vector3 axisUp = toTargetVec.CalcCross(axisSide);

		//1~90の数値を得る
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

	//ダッシュしている時間を計測する
	m_dashTimer += GameTime().GetFrameDeltaTime();
	if (m_dashTimer >= MAX_DASH_TIME)
	{
		m_moveType = enApproach;
		m_dashTimer = 0.0f;
		m_waitTimer = 0.0f;
	}
}

void EnemyMove::EnemyStayDirection(Vector3 toTargetVec)
{
	//漂っている時間を計測
	m_waitTimer += GameTime().GetFrameDeltaTime();
	//規定以上漂っていたら
	if (m_waitTimer >= MAX_WAIT_TIME)
	{
		//ダッシュして移動方向を変える
		m_moveType = enDash;
	}

	//敵の横方向を計算する。
	Vector3 sideVec = toTargetVec.CalcCross(g_vec3AxisY);
	//前の移動方向を加味した漂う方向を計算する。
	m_currentMoveDirection = m_currentMoveDirection * (1.0f - m_acceralation) + sideVec * m_acceralation / SPEED_DECAY_RATE;
}

Vector3 EnemyMove::Execute(Vector3 enemyPos, Vector3 targetPos)
{
	//プレイヤーと敵との距離を計算する。
	float distance = CalcMethods::CalcDistance(enemyPos, targetPos);
	//敵からプレイヤーへの方向を計算する。
	Vector3 toTargetVec = targetPos - enemyPos;
	toTargetVec.Normalize();

	//敵がプレイヤーより離れすぎたら
	if (distance > LIMIT_ENEMY_RECOGNIZE_PLAYER 
		&& (m_moveType != enFrontAndBehind && m_moveType != enAround))
	{
		//接近するようにする。
		m_moveType = enApproach;
	}

	//移動タイプによって計算する移動方向を変える
	switch (m_moveType)
	{
	case enApproach:
		CalcApproachSpeed(toTargetVec, distance);
		break;
	case enFrontAndBehind:
	case enPlayerBehind:
		CalcFrontAndBehindDirection(toTargetVec, distance);
		break;
	case enAround:
		CalcAroundDirection(toTargetVec);
		break;
	case enApproachAndDash:
		CalcApproachAndDashDirection(toTargetVec, distance);
		break;
	case enStay:
		EnemyStayDirection(toTargetVec);
		break;
	case enDash:
		EnemyDashDirection(toTargetVec);
		break;
	default:
		CalcApproachSpeed(toTargetVec, distance);
		break;
	}

	//移動モードが変われば
	if (m_prevMoveType != m_moveType)
	{
		//現在の移動モードを更新する。
		m_prevMoveType = m_moveType;
		//加速度をリセット
		m_acceralation = 0.0f;
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