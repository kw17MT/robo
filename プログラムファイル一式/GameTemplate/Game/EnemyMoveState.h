#pragma once

#include "EnemyStateParam.h"

/**
 * @brief �ړ���Ԃ̊��N���X
*/
class EnemyMoveState 
{
public:
	/**
	 * @brief �f�X�g���N�^
	*/
	virtual ~EnemyMoveState() {}

	/**
	 * @brief �p�����[�^������������
	 * @param param �ړ�����̃p�����[�^
	*/
	virtual void Init(EnemyStateParam &param) = 0;

	/**
	 * @brief �X�e�[�g�̍X�V����
	 * @param param �ړ�����̃p�����[�^
	 * @return �K�p����X�e�[�g
	*/
	virtual EnemyMoveState* Update(EnemyStateParam &param) = 0;
};