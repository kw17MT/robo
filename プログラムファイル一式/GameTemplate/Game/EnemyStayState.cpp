#include "stdafx.h"
#include "EnemyStayState.h"
#include "EnemyDashState.h"

void EnemyStayState::Init(EnemyStateParam &param)
{
	//経過時間をリセットする。
	param.timer = 0.0f;
}

EnemyMoveState* EnemyStayState::Update(EnemyStateParam &param)
{
	//デルタタイムで経過時間を計測
	param.timer += GameTime().GetFrameDeltaTime();
	//待機時間を超えたら
	if (param.timer >= stay::MAX_TIME)
	{
		//ダッシュを行う。
		return EnemyDashState::GetInstance();
	}

	return this;
}