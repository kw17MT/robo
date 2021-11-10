#pragma once
#include "Enemy.h"

enum EnEnemyState
{
	None = 0,
	Captured,
	Targeted,
	ChangeMainTarget
};

//enum EnMissileTargetState
//{
//	enNoTarget,
//	enMissileTarget,
//	enDeleteIcon
//};

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

		//��ԑJ�ڂ̐悪�������b�N�I�����Ă��Ȃ���ԂŁA�v���C���[���\���I��None�ɂ��Ă��Ȃ����
		if (state == None && actively == false)
		{
			//���Ƀ^�[�Q�b�g����G�����܂��Ă�����
			if (m_captureNextEnemy)
			{
				//�����I�Ƀ^�[�Q�b�g����ʒu���W�������ɂ���B
				m_capturedEnemyPos = m_capturedNextEnemyPos;
				//���̓G�����b�N�I�����Ă��邽�߁A��Ԃ����b�N�I����
				m_enemyState = ChangeMainTarget;
				//���̃��b�N�I������ēx���߂������߃t���O�ƍ��W�����ɖ߂��B
				m_captureNextEnemy = false;
				Vector3 homePos = { 0.0f,0.0f,-1.0f };
				m_capturedNextEnemyPos = homePos;
			}
			else
			{
				Vector3 homePos = { 0.0f,0.0f,-1.0f };
				m_capturedEnemyPos = homePos;
			}
		}
	}

	//�ߑ����ꂽ�G�̍��W�ۑ�
	void SetCapturedEnemyPos(Vector3 enemyPos)
	{
		m_capturedEnemyPos = enemyPos;
	}
	//���̓G�Ɏ����I�Ƀ��b�N�I�����ڂ����ǂ���
	void SetNextEnemy(bool state)
	{
		m_captureNextEnemy = state;
	}
	//���Ƀ��b�N�I�����ڂ��G�̍��W��ݒ�
	void SetNextEnemyPos(Vector3 enemyPos)
	{
		m_capturedNextEnemyPos = enemyPos;
	}
	//���̃��b�N�I���Ώې�̍��W�ƃt���O�����Z�b�g����
	void ResetNextEnemyParam()
	{
		m_captureNextEnemy = false;
		m_capturedNextEnemyPos = Vector3::Zero;
	}

	//���P�b�g�̃^�[�Q�b�g�����邩�ǂ�����ݒ肷��
	void SetRocketTargetState(bool state)
	{
		//�����^�[�Q�b�g���ĂȂ��A�V�����^�[�Q�b�g����ꍇ
		if (m_rocketTargetedNum == 0)
		{
			//�ۑ�����^�[�Q�b�g���W�̃��Z�b�g���I����
			m_isResetPosNumber = false;
		}

		//�^�[�Q�b�g������
		if (state)
		{
			//���ǁA�ő吔�ɒB���Ă����Ȃ�
			if (m_rocketTargetedNum >= 9) 
			{
				//�^�[�Q�b�g���Ȃ�
				m_isRocketTargetState = false;
			}
			else
			{
				m_isRocketTargetState = state;
			}
		}
		else
		{
			m_isRocketTargetState = state;
		}
	}

	void PlusRockeTargetNum()
	{
		//�^�[�Q�b�g���鐔���C���N�������g
		m_rocketTargetedNum++; 
		m_isGetRocketTarget = true;
		//�ő吔�ȏ�ɕϐ����C���N�������g�������Ȃ�����
		if (m_rocketTargetedNum == 10)
		{
			m_isGetRocketTarget = false;
			m_rocketTargetedNum = 9;
		}
	}
	void MinusRockeTargetNum()
	{
		//�^�[�Q�b�g���鐔���C���N�������g
		m_rocketTargetedNum--;
		//�ő吔�ȏ�ɕϐ����C���N�������g�������Ȃ�����
		if (m_rocketTargetedNum  <= 0)
		{
			m_rocketTargetedNum = 0;
		}
	}

	void SetRocketTargetPos(Vector3 pos , int num)
	{
		m_rocketTargets[num] = pos;		
	}

	void SetRocketTargetedEnemy(Enemy* enemy)
	{
		m_rocketTargetedEnemy[m_rocketTargetedNum] = enemy;
	}

	void ResetRocketTargetParam()
	{
		for (int i = 1; i < m_rocketTargetedNum; i++)
		{
			Vector3 resetPos = { 0.0f,0.0f,0.0f };
			m_rocketTargets[m_rocketTargetedNum - 1] = resetPos;
		}
		m_rocketTargetedNum = 0;
		m_isResetPosNumber = true;
	}
	void SetIsRocketTargeted(bool state)
	{
		m_isGetRocketTarget = state;
	}

	EnEnemyState GetCaptureState()
	{
		return m_enemyState;
	}
	Vector3 GetCapturedEnemyPos()
	{
		return m_capturedEnemyPos;
	}
	bool IsNextEnemyCaptured()
	{
		return m_captureNextEnemy;
	}
	Vector3 GetCapturedNextEnemyPos()
	{
		return m_capturedNextEnemyPos;
	}
	bool GetRocketTargetState()
	{
		return m_isRocketTargetState;
	}
	int GetRocketTargetNum()
	{
		return m_rocketTargetedNum;
	}
	bool GetResetPosNumber()
	{
		return m_isResetPosNumber;
	}
	bool GetIsRocketTargeted()
	{
		return m_isGetRocketTarget;
	}
	Vector3 GetRocketTargetEnemyPos(int number)
	{
		return m_rocketTargets[number];
	}

	Enemy* GetRocketTargetEnemy(int number)
	{
		return m_rocketTargetedEnemy[number];
	}

private:
	EnEnemyState m_enemyState = None;
	Vector3 m_capturedEnemyPos = Vector3::Zero;

	bool m_captureNextEnemy = false;
	Vector3 m_capturedNextEnemyPos = Vector3::Zero;

	bool m_isRocketTargetState = false;
	int m_rocketTargetedNum = 0;
	std::array<Vector3, 10> m_rocketTargets;
	std::array<Enemy*, 10> m_rocketTargetedEnemy;

	bool m_isResetPosNumber = false;
	bool m_isGetRocketTarget = false;
};

