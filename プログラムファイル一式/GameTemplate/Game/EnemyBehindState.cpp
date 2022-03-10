#include "stdafx.h"
#include "EnemyBehindState.h"
#include "EnemyDashState.h"
#include "EnemyApproachState.h"

void EnemyBehindState::Init(EnemyStateParam &param)
{
	//時間をリセット
	param.timer = 0.0f;
	//背後をとり続けるようにする
	param.getBehind = true;
}

EnemyMoveState* EnemyBehindState::Update(EnemyStateParam &param)
{
	//プレイヤーが見ている向きと敵からプレイヤーへの方向の内積を計算する。
	float dot = param.toTargetDirection.Dot(g_camera3D->GetForward());
	//内積が一定以下ならば見られていると判断する
	if (dot < behind::DOT_RECOGNIZE_PLAYER)
	{
		//ダッシュ状態にする。
		return EnemyDashState::GetInstance();
	}
	else
	{
		//繰り返し背後をとるモードをやめる。
		param.getBehind = false;
		//接近状態にする。
		return EnemyApproachState::GetInstance();
	}

	return this;
}