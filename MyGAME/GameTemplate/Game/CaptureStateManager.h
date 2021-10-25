#pragma once

enum EnEnemyState
{
	None = 0,
	Captured,
	Targeted,
	ChangeMainTarget
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
	void SetCapturedEnemyPos(Vector3 enemyPos)
	{
		m_capturedEnemyPos = enemyPos;
	}
	void SetNextEnemy(bool state)
	{
		m_captureNextEnemy = state;
	}
	void SetNextEnemyPos(Vector3 enemyPos)
	{
		m_capturedNextEnemyPos = enemyPos;
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

private:
	EnEnemyState m_enemyState = None;
	Vector3 m_capturedEnemyPos = Vector3::Zero;

	bool m_captureNextEnemy = false;
	Vector3 m_capturedNextEnemyPos = Vector3::Zero;
};

