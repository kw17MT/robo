#include "stdafx.h"
#include "EnemyBackState.h"
#include "EnemyApproachState.h"
#include "EnemyAroundState.h"

namespace
{
	const int USE_STATE_NUM = 2;						//ここで使用する状態の数
}

void EnemyBackState::Init(EnemyStateParam &param)
{
	//経過時間をリセット
	param.timer = 0.0f;
}

EnemyMoveState* EnemyBackState::Update(EnemyStateParam &param)
{
	//敵とプレイヤーが離れすぎると
	if (param.distanceBetweenEnemyToPlayer >= approach::LIMIT_RECOGNIZE)
	{
		//ランダムで接近状態にするか、旋回状態にするか選択する。
		srand((unsigned int)time(NULL));
		int nextMoveState = rand() % USE_STATE_NUM;
		if (nextMoveState)
		{
			//接近
			return EnemyApproachState::GetInstance();
		}
		else if (!nextMoveState)
		{
			//旋回
			return EnemyAroundState::GetInstance();
		}
	}

	return this;
}