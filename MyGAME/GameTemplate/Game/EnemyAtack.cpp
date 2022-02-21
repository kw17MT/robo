#include "stdafx.h"
#include "EnemyAtack.h"
#include "SkinModelRender.h"

namespace
{
	const float SHOOT_TIME = 3.0f;
}

bool EnemyAtack::Execute()
{
	//デルタタイムを用いて発射間隔を計測
	counter += GameTime().GetFrameDeltaTime();
	//一定間隔空いたので発射できることを返す。
	if (counter >= SHOOT_TIME)
	{
		counter = 0.0f;
		return true;
	}
	return false;
}