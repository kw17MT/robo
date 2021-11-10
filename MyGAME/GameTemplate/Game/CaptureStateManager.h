#pragma once
#include "Enemy.h"

enum EnEnemyState
{
	None,
	Captured,
	Targeted,
	ChangeMainTarget
};

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
	static CaptureStateManager* instance;
public:
	static void CreateInstance()
	{
		instance = new CaptureStateManager;
	}
	static CaptureStateManager& GetInstance()
	{
		static CaptureStateManager instance;
		return instance;
	}
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief �v���C���[�̃��b�N�I����Ԃ�ύX�B���̓G�������I�Ƀ��b�N�I��
	 * @param state �v���C���[�̃��b�N�I�����
	 * @param actively �v���C���[�����b�N�I���������ŉ����������ǂ���
	*/
	void SetCaptureState(EnEnemyState state, bool actively = true)
	{
		m_enemyState = state;
	}

	EnEnemyState GetCaptureState()
	{
		return m_enemyState;
	}

	//���P�b�g�̃^�[�Q�b�g�����邩�ǂ�����ݒ肷��
	void SetMissileTargetState(EnMissileTargetState state)
	{
		m_isMissileTargetState = state;
	}

	EnMissileTargetState GetMissileTargetState()
	{
		return m_isMissileTargetState;
	}

private:
	EnEnemyState m_enemyState = None;
	EnMissileTargetState m_isMissileTargetState = enNoTarget;
};