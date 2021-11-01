#pragma once
#include "SpriteRender.h"

class Reticle : public IGameObject
{
private:
	//スクリーン画面のホームポジション
	const Vector3 m_homePosition = { 0.5f,0.5f,0.0f };
	//敵をロックオンした時のポジション
	Vector3 m_lockOnPosition = Vector3::Zero;
	//レティクルの拡大率（1が通常レティクル、2がターゲット
	Vector3 m_reticleScale[2] = { Vector3::One, Vector3::One };

	//CaptureStateManager* m_stateManager = &CaptureStateManager::GetInstance();

	bool m_isTarget = false;
	float m_PressButtonTime = 0.5f;
public:
	Reticle() {};
	~Reticle();
	bool Start();
	void Update();
	void CalcPosition();
	void ChangeTargetStateDependOnButtonLB2();
private:
	std::array< SpriteRender*, 2> m_spriteRender = { nullptr, nullptr };
};