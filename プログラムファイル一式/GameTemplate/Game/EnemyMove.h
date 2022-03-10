#pragma once
#include "EnemyApproachState.h"

class EnemyMove
{
private:
	bool m_isCalcEnemyDashDirection = false;			//�G�̓_�b�V���������邩
	float m_moveSpeed = 0.0f;							//���̓G�̈ړ����x
	float m_acceralation = 1.0f;						//�����x
	Vector3 m_currentMoveDirection = Vector3::Zero;		//���݂̈ړ�����
	Vector3 m_dashDirection = Vector3::Zero;			//�_�b�V���̕���
	EnemyStateParam m_param;							//�ړ��J�ڂɎg�p����p�����[�^
	EnemyMoveState* m_state;							//���݂̈ړ����

public:
	/**
	 * @brief �R���X�g���N�^�A�ړ����[�h��ڋ߂ŏ�����
	*/
	EnemyMove() { m_state = EnemyApproachState::GetInstance();}

	/**
	 * @brief �G�̈ړ����x��ݒ�
	 * @param moveSpeed	���j�[�N�Ȉړ����x
	*/
	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	 * @brief �G�̌��݂̈ړ��������擾����
	 * @return �G�̈ړ�����
	*/
	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief �ڋߕ������v�Z����
	 * @param toTargetVec �^�[�Q�b�g�ւ̕���
	*/
	void CalcApproachDirection(Vector3 toTargetVec);

	/**
	 * @brief ��ޕ������v�Z����
	 * @param toTargetVec �^�[�Q�b�g�ւ̕���
	*/
	void CalcBackDirection(Vector3 toTargetVec);

	/**
	* @brief ����������v�Z����
	* @param toTargetVec �^�[�Q�b�g�ւ̕���
	*/
	void CalcAroundDirection(Vector3 toTargetVec);

	/**
	* @brief �_�b�V���������v�Z����
	* @param toTargetVec �^�[�Q�b�g�ւ̕���
	*/
	void CalcDashDirection(Vector3 toTargetVec);

	/**
	* @brief �������蓮���������v�Z����
	* @param toTargetVec �^�[�Q�b�g�ւ̕���
	*/
	void CalcStayDirection(Vector3 toTargetVec);

	/**
	* @brief �ړ����[�h�ɂ���Ď��̍��W���v�Z����B
	* @param enemypos ���݂̈ʒu���W
	* @param targetPos �^�[�Q�b�g�ƂȂ�I�u�W�F�N�g�̈ʒu���W
	* @return ���̈ʒu���W
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);
};

