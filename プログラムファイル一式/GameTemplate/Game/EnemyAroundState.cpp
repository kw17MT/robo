#include "stdafx.h"
#include "EnemyAroundState.h"
#include "EnemyApproachState.h"

void EnemyAroundState::Init(EnemyStateParam &param)
{
	//経過時間をリセットする
	param.timer = 0.0f;
}

EnemyMoveState* EnemyAroundState::Update(EnemyStateParam &param)
{
	//デルタタイムで計測
	param.timer += GameTime().GetFrameDeltaTime();
	//一定時間旋回したら
	if (param.timer >= around::MAX_TIME)
	{
		//接近状態に遷移する
		return EnemyApproachState::GetInstance();
	}

	return this;
}