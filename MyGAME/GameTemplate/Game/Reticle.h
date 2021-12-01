#pragma once
#include "SpriteRender.h"

enum EnReticleType
{
	enNormalReticle,
	enTargetReticle
};

class Reticle : public IGameObject
{
private:
	//�X�N���[����ʂ̃z�[���|�W�V����
	const Vector3 m_homePosition = { 0.5f,0.5f,0.0f };
	//�G�����b�N�I���������̃|�W�V����
	Vector3 m_lockOnPosition = Vector3::Zero;
	//���e�B�N���̊g�嗦�i1���ʏ탌�e�B�N���A2���^�[�Q�b�g
	Vector3 m_reticleScale[2] = { Vector3::Zero, Vector3::Zero };
	//�^�[�Q�b�g���Ă���G�̈ʒu
	Vector3 m_targetingEnemyPos = Vector3::Zero;

	bool m_isTarget = false;				//���݃^�[�Q�b�g���Ă��邩
	//bool m_isDecidedNextTarget = false;		//���Ƀ^�[�Q�b�g����G�͌��܂��Ă��邩�i�G�����m���Ă���j
	float m_PressButtonTime = 0.5f;			//LB2�{�^���������Ă���b��
	float m_targetReticleRotateRate = 0.0f;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Reticle() {};

	/**
	 * @brief �f�X�g���N�^ 
	*/
	~Reticle();

	/**
	 * @brief �I�u�W�F�N�g�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();


	void CalcPosition();

	/**
	 * @brief LB2�{�^���̉��������i�ǂꂾ�����������j�Ń^�[�Q�b�g�̎�ނ����ʂ���
	*/
	void ChangeTargetStateDependOnButtonLB2();

	/**
	 * @brief ���݃^�[�Q�b�g���Ă��邩�ݒ肷��
	 * @param state ���݃^�[�Q�b�g���Ă��邩
	*/
	void SetIsTargeting(bool state) { m_isTarget = state; }

	//void SetIsDecidedNextTarget(bool isDecided) { m_isDecidedNextTarget = isDecided; }

	void SetTargetedEnemyPos(Vector3 pos) { m_targetingEnemyPos = pos; }

	/**
	 * @brief ���݃^�[�Q�b�g���Ă��邩
	 * @return TRUE���Ă���@FALSE���ĂȂ�
	*/
	bool GetIsTargeted() { return m_isTarget; }

	//bool GetIsDecidedNextTarget() { return m_isDecidedNextTarget; }

	Vector3 GetTargetingEnemyPos() { return m_targetingEnemyPos; }

private:
	std::array< SpriteRender*, 2> m_spriteRender = { nullptr, nullptr };
};