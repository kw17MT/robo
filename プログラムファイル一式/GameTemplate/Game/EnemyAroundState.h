#pragma once
#include "EnemyMoveState.h"

class EnemyAroundState : public EnemyMoveState
{
public:
	/**
	* @brief �C���X�^���X���擾����
	* @return �ڋߏ��
	*/
	static EnemyAroundState* GetInstance()
	{
		static EnemyAroundState instance;
		return &instance;
	}

	/**
	 * @brief �p�����[�^������������
	 * @param param �ړ�����̃p�����[�^
	*/
	virtual void Init(EnemyStateParam &param);

	/**
	* @brief �X�e�[�g�̍X�V����
	* @param param �ړ�����̃p�����[�^
	* @return �K�p����X�e�[�g
	*/
	virtual EnemyMoveState* Update(EnemyStateParam &param);
};