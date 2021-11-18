#include "stdafx.h"
#include "EnemyAtack.h"
#include "SkinModelRender.h"

bool EnemyAtack::Execute()
{
	counter++;
	if (counter >= 100)
	{
		counter = 0;
		return true;
	}
	return false;
}