#include "stdafx.h"
#include "PlayerAnimation.h"

void PlayerAnimation::UpdateAnimState()
{
	//�}�V���K���A�j���[�V����
	if (g_pad[0]->IsPress(enButtonRB2))
	{
		m_animState = enShooting;
		return;
	}
	//�㏸�A�j���[�V����
	if (g_pad[0]->IsPress(enButtonY))
	{
		m_animState = enUp;
		return;
	}
	//���~�A�j���[�V����
	else if(g_pad[0]->IsPress(enButtonA))
	{

	}
	//�O�i�A�j���[�V����
	if (g_pad[0]->GetLStickXF()) {
		if (g_pad[0]->GetLStickYF() > 0.0f)
		{
			m_animState = enFlying;
			return;
		}
		else if (g_pad[0]->GetLStickYF() < -0.7f)
		{
			m_animState = enBacking;
			return;
		}
	}
	//�X�e�B�b�N��G���Ă��Ȃ����
	else if(!g_pad[0]->GetLStickXF())
	{
		//�ҋ@�A�j���[�V����
		m_animState = enIdle;
		return;
	}
}