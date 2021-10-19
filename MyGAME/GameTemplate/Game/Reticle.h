#pragma once
#include "SpriteRender.h"

class Reticle : public IGameObject
{
private:
	//�X�N���[����ʂ̃z�[���|�W�V����
	const Vector3 m_homePosition = { 0.5f,0.5f,0.0f };
	//�G�����b�N�I���������̃|�W�V����
	Vector3 m_lockOnPosition = Vector3::Zero;
	//���e�B�N���̊g�嗦�i1���ʏ탌�e�B�N���A2���^�[�Q�b�g
	Vector3 m_reticleScale[2] = { Vector3::One, Vector3::One };

	void CalcPosition();

	bool m_isTarget = false;

public:
	bool Start();
	void Update();
private:
	SpriteRender* m_spriteRender[2] = { nullptr, nullptr };
};