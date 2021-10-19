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

	void CalcPosition();

	bool m_isTarget = false;

public:
	bool Start();
	void Update();
private:
	SpriteRender* m_spriteRender[2] = { nullptr, nullptr };
};