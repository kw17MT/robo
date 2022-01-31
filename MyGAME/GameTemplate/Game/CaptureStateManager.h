#pragma once
#include "Enemy.h"

/**
 * @brief ���e�B�N���̕ߑ��A�^�[�Q�b�g���
*/
enum EnReticleState
{
	None,
	Captured,
	Targeted,
	ChangeMainTarget
};

/**
 * @brief ���e�B�N���̃~�T�C���^�[�Q�b�g���
*/
enum EnMissileTargetState
{
	enNoTarget,
	enMissileTarget,
	enMissileTargeted,
	enFull
};

class CaptureStateManager
{
private:
	//�V���O���g���C���X�^���X
	static CaptureStateManager* instance;
public:
	/**
	 * @brief �C���X�^���X�̍쐬
	*/
	static void CreateInstance()
	{
		instance = new CaptureStateManager;
	}

	/**
	 * @brief �C���X�^���X�̎擾
	*/
	static CaptureStateManager& GetInstance()
	{
		static CaptureStateManager instance;
		return instance;
	}

	/**
	 * @brief �C���X�^���X�̍폜
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief �v���C���[�̃��b�N�I����Ԃ�ύX�B���̓G�������I�Ƀ��b�N�I��
	 * @param state �v���C���[�̃��b�N�I�����
	 * @param actively �v���C���[�����b�N�I���������ŉ����������ǂ���
	*/
	void SetCaptureState(EnReticleState state, bool actively = true)
	{
		m_enemyState = state;
	}

	/**
	 * @brief �ߑ���Ԃ��擾����
	 * @return �ߑ����
	*/
	EnReticleState GetCaptureState()
	{
		return m_enemyState;
	}

	/**
	 * @brief ���e�B�N�����~�T�C���Ń^�[�Q�b�g���Ă��邩�ǂ����ݒ肷��
	 * @param state �^�[�Q�b�g���Ă��邩�ǂ���
	*/
	void SetMissileTargetState(EnMissileTargetState state)
	{
		m_isMissileTargetState = state;
	}

	/**
	 * @brief �~�T�C���̃^�[�Q�b�g��Ԃ��擾����
	 * @return �~�T�C���̃^�[�Q�b�g���
	*/
	EnMissileTargetState GetMissileTargetState()
	{
		return m_isMissileTargetState;
	}

	/**
	 * @brief ���̃^�[�Q�b�g���ݒ肷��
	 * @param isDecided �����^�[�Q�b�g���Ă��邩�ǂ���
	*/
	void SetIsDecidedNextTarget(bool isDecided)
	{
		m_isDecidedNextTarget = isDecided;
	}

	/**
	 * @brief �����^�[�Q�b�g���Ă��邩�ǂ������擾����
	*/
	bool GetIsDecidedNextTarget()
	{
		return m_isDecidedNextTarget;
	}

private:
	EnReticleState m_enemyState = None;								//���e�B�N���̃^�[�Q�b�g���
	EnMissileTargetState m_isMissileTargetState = enNoTarget;			//�~�T�C���̃^�[�Q�b�g���
	bool m_isDecidedNextTarget = false;									//���̓G���^�[�Q�b�g���Ă��邩�ǂ���
};	