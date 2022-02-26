#pragma once

enum EnEnemyMoveTypes
{
	enApproach = -1,			//�v���C���[�ɂ��킶��ڋ߂���
	enFrontAndBehind,			//�ڋߌ�A�O��Ɉړ�����
	enPlayerBehind,				//�v���C���[�̌������悤�ɒʉ߂ƑO����J��Ԃ�
	enAround,					//�v���C���[�̎����ڋߌ�_�b�V���ł������񉓂���
	enApproachAndDash,			//�ڋߌ�A�v���C���[�������悤�ɑO���Ƀ_�b�V��
	enStay,						//�ڋߌ�A���̏�őؗ����A�����Ԍ�_�b�V���ő��̕�����
	enDash						//�v���C���[�Ƃ���Ⴄ�悤�Ƀ_�b�V������B
};

class EnemyMove
{
private:
	EnEnemyMoveTypes m_moveType = enApproach;			//���݂̈ړ��^�C�v
	EnEnemyMoveTypes m_prevMoveType = enApproach;		//�O�܂ł̈ړ��^�C�v

	bool m_moveForward = true;							//�O��̓����̒��őO�i���邩�ǂ���
	bool m_isCalcEnemyDashDirection = false;			//�G�̓_�b�V���������邩

	float m_dashTimer = 0.0f;							//�_�b�V�����Ă��鎞��
	float m_aroundTimer = 0.0f;							//�v���C���[���S�ɐ��񂵂Ă��鎞��
	float m_waitTimer = 0.0f;							//���̏�Ńv���C���[�𒍎����Ă��鎞��
	float m_moveSpeed = 0.0f;							//���̓G�̈ړ����x
	float m_acceralation = 1.0f;						//�����x

	Vector3 m_currentMoveDirection = Vector3::Zero;		//���݂̈ړ�����
	Vector3 m_dashDirection = Vector3::Zero;			//�_�b�V���̕���
public:
	/**
	 * @brief �G�̈ړ����x��ݒ�
	 * @param moveSpeed	���j�[�N�Ȉړ����x
	*/
	void SetMoveSpeed(const float moveSpeed) { m_moveSpeed = moveSpeed; }

	/**
	* @brief ���݂̓G�̈ړ����@���擾����
	* @param type �ړ����@
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief �G�̌��݂̈ړ��������擾����
	 * @return �G�̈ړ�����
	*/
	Vector3 GetEnemyMoveDireciton() { return m_currentMoveDirection; }

	/**
	 * @brief �v���C���[�ɐڋ߃��[�h�̓G�̎��̈ʒu���W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���ɂ̈ʒu���W
	*/
	void CalcApproachSpeed(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief �ړ����[�h�ɂ���Ď��̍��W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���̈ʒu���W
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief �v���C���[�Ɍ������đO��^������Ƃ��̈ړ��������v�Z����B
	 * @param toTargetVec �v���C���[�ւ̕����x�N�g��
	 * @param distanceBetweenEnemyToPlayer �G�ƃv���C���[�̋���
	*/
	void CalcFrontAndBehindDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief ���񂷂�Ƃ��̈ړ��������v�Z����B
	 * @param toTargetVec �v���C���[�ւ̕����x�N�g��
	*/
	void CalcAroundDirection(Vector3 toTargetVec);

	/**
	 * @brief �ڋ߂��ă_�b�V���Ńv���C���[��ʂ�߂���ړ��������v�Z����B
	 * @param toTargetVec �v���C���[�ւ̕����x�N�g��
	 * @param distanceBetweenEnemyToPlayer �G�ƃv���C���[�̋���
	*/
	void CalcApproachAndDashDirection(Vector3 toTargetVec, float distanceBetweenEnemyToPlayer);

	/**
	 * @brief �_�b�V���������v�Z����B
	 * @param toTargetVec �v���C���[�ւ̕����x�N�g��
	*/
	void EnemyDashDirection(Vector3 toTargetVec);

	/**
	 * @brief �G���v���C���[�̎����Y���Ƃ��̈ʒu���W�̍X�V
	 * @param toTargetVec �v���C���[�ւ̕����x�N�g��
	*/
	void EnemyStayDirection(Vector3 toTargetVec);

	/**
	 * @brief �G�ƃv���C���[�̈ʒu�֌W�ɂ���Ăǂ̂悤�Ɉړ����邩���߂�
	 * @param distanceBetweenEnemyToPlayer �G�ƃv���C���[�̋���
	*/
	void JudgeMoveType(float distanceBetweenEnemyToPlayer);
};

