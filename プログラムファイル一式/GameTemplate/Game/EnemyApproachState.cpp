#include "stdafx.h"
#include "EnemyApproachState.h"
#include "EnemyDashState.h"
#include "EnemyAroundState.h"
#include "EnemyStayState.h"
#include "EnemyBehindState.h"
#include "EnemyBackState.h"

namespace
{
	const int USE_STATE_NUM = 4;		//ここで使用する別のステートの数
}

void EnemyApproachState::Init(EnemyStateParam &param)
{
	//経過時間をリセット
	param.timer = 0.0f;
}

EnemyMoveState* EnemyApproachState::Update(EnemyStateParam &param)
{
	//プレイヤーに限界まで近づいたら
	if (param.distanceBetweenEnemyToPlayer < approach::LIMIT_APPROACH)
	{
		//ランダムに移動ステートを選択する
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % USE_STATE_NUM;

		if (nextMoveState == stateNo::DASH)
		{
			//ダッシュ状態
			return EnemyDashState::GetInstance();
		}
		else if (nextMoveState == stateNo::AROUND)
		{
			//旋回状態
			return EnemyAroundState::GetInstance();
		}
		else if (nextMoveState == stateNo::BACK)
		{
			//後退状態
			return EnemyBackState::GetInstance();
		}
		else if (nextMoveState == stateNo::BEHIND)
		{
			//プレイヤーの背後を取る状態
			return EnemyBehindState::GetInstance();
		}
	}

	return this;
}