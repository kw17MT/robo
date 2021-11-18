#pragma once
#include "EnemyAtack.h"
#include "EnemyMove.h"
#include "EnemyRotation.h"

class EnemyBrain
{
private:
	Vector3 m_playerPos = Vector3::Zero;		//�v���C���[�̈ʒu
	Vector3 m_enemyPos = Vector3::Zero;			//�G�i�����j�̈ʒu
	Quaternion m_rot = Quaternion::Identity;

	bool m_canShoot = false;
public:
	/**
	 * @brief �G�l�~�[�̐������ɍ쐬�����ړ����x��ݒ肷��B
	 * @param moveSpeed �����_���Ȉړ����x
	*/
	void SetMoveSpeed(const float moveSpeed) { m_enemyMove.SetMoveSpeed(moveSpeed); }
	/**
	 * @brief �v���C���[�̈ʒu���W���L������
	 * @param playerPos �v���C���[�̈ʒu���W
	*/
	void MemoryPlayerPos(Vector3 playerPos) { m_playerPos = playerPos; }
	
	/**
	 * @brief �G�i�����j�̈ʒu���W���L������B
	 * @param enemyPos �G�i�����̈ʒu���W
	*/
	void MemoryEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }

	/**
	 * @brief ��Ɍv�Z�ς݂̓G�i�����j�̈ʒu���擾����
	 * @return �G�i�����j�̈ʒu
	*/
	Vector3 GetNextEnemyPos() { return m_enemyPos; }

	Quaternion GetEnemyRotation() { return m_rot; }

	/**
	 * @brief �ړ����[�h�𔻒f����B
	*/
	//void JudgeMoveType();

	bool JudgeCanShoot();

	/**
	 * @brief ���𓭂����B���ׂĂ̔��f����C�ɍs��
	*/
	void Work();
private:
	EnemyAtack m_enemyAtack;			//�U�����邩�ǂ����̔��f
	EnemyMove m_enemyMove;				//�ړ�����l����
	EnemyRotation m_enemyRotation;
};

