#pragma once

enum EnEnemyMoveTypes
{
	enApproach,					//�v���C���[�ɂ��킶��ڋ߂���
	enFrontAndBehind,			//�ڋߌ�A�O��Ɉړ�����
	enPlayerBehind,				//�v���C���[�̌������悤�ɒʉ߂ƑO����J��Ԃ�
	enAround,					//�v���C���[�̎����ڋߌ�_�b�V���ł������񉓂���
	enApproachAndDash,			//�ڋߌ�A�v���C���[�������悤�ɑO���Ƀ_�b�V��
	enStay,						//�ڋߌ�A���̏�őؗ����A�����Ԍ�_�b�V���ő��̕�����
	enDash
};

enum EnEnemyAltitudeState
{
	enTooHigh,
	enTooLow,
	enSafe
};

class EnemyMove
{
private:
	float m_moveSpeed = 0.0f;
	bool m_isTop = false;
	EnEnemyMoveTypes m_moveType = enApproach;
	EnEnemyMoveTypes m_prevMoveType = enApproach;
	EnEnemyAltitudeState m_altitudeState = enSafe;
	float m_adjustingAltitudeElapsedTime = 0.0f;
	float m_adjustedAltitudeHeight = 0.0f;				//���x�����p
	Vector3 m_prevMoveDirection = Vector3::Zero;


	bool m_moveForward = true;							//�O��̓����̒��őO�i���邩�ǂ���
	bool m_isCalcEnemyDashDirection = false;							//�G�̓_�b�V���������邩
	float m_dashTimer = 0.0f;
	float m_aroundTimer = 0.0f;
	float m_waitTimer = 0.0f;

	float m_acceralation = 1.0f;

	Vector3 m_currentMoveDirection = Vector3::Zero;

public:

	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	 * @brief ���݂̓G�̈ړ����@��ݒ肷��
	 * @param type �ړ����@
	*/
	void SetEnemyMoveTypes(EnEnemyMoveTypes type) { m_moveType = type; }

	/**
	 * @brief ���݂̓G�̍��x�̏�Ԃ�ݒ肷��
	 * @param state ���x�̒��x
	*/
	void SetEnemyAltitudeState(EnEnemyAltitudeState state) { m_altitudeState = state; }

	/**
	* @brief ���݂̓G�̈ړ����@���擾����
	* @param type �ړ����@
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief ���݂̓G�̍��x�̏�Ԃ��擾����
	 * @return ���x�̒��x
	*/
	EnEnemyAltitudeState GetEnemyAltitudeState() { return m_altitudeState; }

	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief �v���C���[�ɐڋ߃��[�h�̓G�̎��̈ʒu���W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���ɂ̈ʒu���W
	*/
	void CalcApproachSpeed(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief �G�̕W���ɉ����Đ���ȍ����ɖ߂�
	*/
	Vector3 AdjustedAltitudeSpeed(Vector3 calcedPos);

	/**
	 * @brief �ړ����[�h�ɂ���Ď��̍��W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���̈ʒu���W
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);

	void CalcFrontAndBehindSpeed(Vector3 enemyPos, Vector3 targetPos);

	void CalcAroundSpeed(Vector3 enemyPos, Vector3 targetPos);

	void CalcApproachAndDashSpeed(Vector3 enemyPos, Vector3 targetPos);

	void EnemyDashSpeed(Vector3 enemyPos, Vector3 targetPos, float dashTime = 5.0f);

	void EnemyStaySpeed(Vector3 enemyPos, Vector3 targetPos);

	void JudgeMoveType(Vector3 enemyPos, Vector3 targetPos);
private:
};

