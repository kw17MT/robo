#include "stdafx.h"
#include "EnemyAtack.h"
#include "SkinModelRender.h"

namespace
{
	const float SHOOT_TIME = 3.0f;
}

bool EnemyAtack::Execute()
{
	counter += GameTime().GetFrameDeltaTime();
	if (counter >= SHOOT_TIME)
	{
		counter = 0.0f;
		return true;
	}
	return false;
}