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