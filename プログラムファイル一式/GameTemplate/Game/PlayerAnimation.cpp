#include "stdafx.h"
#include "PlayerAnimation.h"

namespace
{
	const float INPUT_STICK_Y_VOLUME_TO_START = -0.7f;
}

void PlayerAnimation::UpdateAnimState()
{
	//マシンガンアニメーション
	if (g_pad[0]->IsPress(enButtonRB2))
	{
		m_animState = enShooting;
		return;
	}
	//上昇アニメーション
	if (g_pad[0]->IsPress(enButtonY))
	{
		m_animState = enUp;
		return;
	}
	//下降アニメーション
	else if(g_pad[0]->IsPress(enButtonA))
	{

	}
	//前進アニメーション
	if (g_pad[0]->GetLStickXF()) {
		if (g_pad[0]->GetLStickYF() > 0.0f)
		{
			m_animState = enFlying;
			return;
		}
		else if (g_pad[0]->GetLStickYF() < INPUT_STICK_Y_VOLUME_TO_START)
		{
			m_animState = enBacking;
			return;
		}
	}
	//スティックを触っていなければ
	else if(!g_pad[0]->GetLStickXF())
	{
		//待機アニメーション
		m_animState = enIdle;
		return;
	}
}