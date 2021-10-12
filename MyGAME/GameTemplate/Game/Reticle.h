#pragma once
#include "SpriteRender.h"

class Reticle : public IGameObject
{
private:
	const Vector3 m_homePosition = { 0.5f,0.5f,0.0f };
	Vector2 m_lockOnPosition = Vector2::Zero;

	Vector3 m_reticleScale[2] = { Vector3::One, Vector3::One };


	int m_isTarget = 0;


public:
	bool Start();
	void Update();
private:
	SpriteRender* m_spriteRender[2] = { nullptr, nullptr };
};