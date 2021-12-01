#pragma once
class Effect;

namespace
{
	const int EFFECT_NUM = 4;
	const int BOOSTER_EFFECT_FRAME = 60;
}

class PlayerEffect : public IGameObject
{
private:
	Vector3 m_boosterRightPos = Vector3::Zero;
	Vector3 m_boosterLeftPos = Vector3::Zero;
	Vector3 m_boosterScale = Vector3::Zero;
	Vector3 m_shoulderRightPos = Vector3::Zero;
	Vector3 m_shoulderLeftPos = Vector3::Zero;
	Quaternion m_boosterEffectRot = Quaternion::Identity;
	Quaternion m_aircontrailEffectRot = Quaternion::Identity;
	int m_effectTimer = 0;
	bool m_isDash = false;
	bool m_isPlayerDied = false;
	float m_playerMoveSpeed = 0.0f;
public:
	PlayerEffect() {}
	~PlayerEffect();
	bool Start();
	void Update();

	void CalcRotation(Vector3 playerMoveDirection);

	void SetBothBoosterPos(Vector3 boosterRPos, Vector3 boosterLPos)
	{
		m_boosterRightPos = boosterRPos;
		m_boosterLeftPos = boosterLPos;
	}

	void SetBothShoulderPos(Vector3 shoulderRPos, Vector3 shoulderLPos)
	{
		m_shoulderRightPos = shoulderRPos;
		m_shoulderLeftPos = shoulderLPos;
	}

	void SetIsDash(bool isDash) { m_isDash = isDash; }

	void SetIsDied(bool isDied) { m_isPlayerDied = isDied; }
private:
	Effect* m_effects[EFFECT_NUM] = { nullptr };
	Effect* m_dieEffect = nullptr;
};

