#pragma once

enum EnPlayerAnimationState
{
	enIdle,
	enForwardLeaning,
	enFlying,
	enShooting,
	enBackLeaning,
	enBacking,
	enUp
};

class PlayerAnimation
{
private:
	EnPlayerAnimationState m_animState = enIdle;
	bool m_isFinishForwardLeaning = false;
public:
	void UpdateAnimState();
	EnPlayerAnimationState GetPlayerState() { return m_animState; }
private:
};

