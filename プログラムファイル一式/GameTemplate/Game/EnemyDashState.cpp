#include "stdafx.h"
#include "EnemyDashState.h"
#include "EnemyApproachState.h"
#include "EnemyBehindState.h"

void EnemyDashState::Init(EnemyStateParam &param)
{
	//経過時間をリセット
	param.timer = 0.0f;
}

EnemyMoveState* EnemyDashState::Update(EnemyStateParam &param)
{
	//デルタタイムで時間を計測
	param.timer += GameTime().GetFrameDeltaTime();
	//ダッシュ時間が最大時間を超えたら
	if (param.timer >= dash::MAX_TIME)
	{
		//プレイヤーは敵を見ているか内積を使って判断する
		float dot = param.toTargetDirection.Dot(g_camera3D->GetForward());
		//見ていて、繰り返し背後を取るモードであったら
		if (dot < behind::DOT_RECOGNIZE_PLAYER
			&& param.getBehind)
		{
			//背後を取る状態にする
			return EnemyBehindState::GetInstance();
		}
		//接近状態にする
		return EnemyApproachState::GetInstance();
	}

	return this;
}