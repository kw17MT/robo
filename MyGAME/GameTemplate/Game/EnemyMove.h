#pragma once

enum EnEnemyMoveTypes
{
	enNormalMove,
	enApproach,
	enRun,
	enAroundPlayer
};

class EnemyMove
{
private:
	bool m_isTop = false;
	EnEnemyMoveTypes m_moveType = enNormalMove;
	const Vector3 m_homePos = { 0.0f, 0.0f, 0.0f };
	Vector3 m_prevMoveDirection = Vector3::Zero;
public:
	/**
	 * @brief ���݂̓G�̈ړ����@��ݒ肷��
	 * @param type �ړ����@
	*/
	void SetEnemyMoveTypes(EnEnemyMoveTypes type) { m_moveType = type; }

	/**
	* @brief ���݂̓G�̈ړ����@���擾����
	* @param type �ړ����@
	*/
	EnEnemyMoveTypes GetEnemyMoveTypes() { return m_moveType; }

	/**
	 * @brief �^�[�Q�b�g�̈ʒu�𒆐S�ɉ��ړ����[�h�̎��̎��̈ʒu���W���v�Z����
	 * @param enemyPos ���݂̈ʒu���W
	 * @param targetPos ��钆�S�ɂ�����W�i�����w�肵�Ȃ��Ƃ��͌��_�𒆐S�ɉ񂷁j
	 * @return ���̈ʒu���W
	*/
	Vector3 CalcNextPos(Vector3 enemyPos, Vector3 targetPos = { 0.0f,0.0f,0.0f });

	/**
	 * @brief �v���C���[�ɐڋ߃��[�h�̓G�̎��̈ʒu���W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���ɂ̈ʒu���W
	*/
	Vector3 CalcApproachedPos(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief �v���C���[���瓦���郂�[�h�̓G�̎��̈ʒu���W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���̈ʒu���W
	*/
	Vector3 CalcRunPos(Vector3 enemyPos, Vector3 targetPos);

	/**
	 * @brief �ړ����[�h�ɂ���Ď��̍��W���v�Z����B
	 * @param pos ���݂̈ʒu���W
	 * @return ���̈ʒu���W
	*/
	Vector3 Execute(Vector3 enemyPos, Vector3 targetPos);
private:
};

