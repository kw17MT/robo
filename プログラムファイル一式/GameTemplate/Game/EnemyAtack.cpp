#include "stdafx.h"
#include "EnemyAtack.h"
#include "SkinModelRender.h"

namespace
{
	const float SHOOT_TIME = 3.0f;
}

bool EnemyAtack::Execute()
{
	//�f���^�^�C����p���Ĕ��ˊԊu���v��
	counter += GameTime().GetFrameDeltaTime();
	//���Ԋu�󂢂��̂Ŕ��˂ł��邱�Ƃ�Ԃ��B
	if (counter >= SHOOT_TIME)
	{
		counter = 0.0f;
		return true;
	}
	return false;
}