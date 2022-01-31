#pragma once
class EnemyRepopManager
{
private:
	static EnemyRepopManager* instance;			//�V���O���g���C���X�^���X
	bool m_shouldRepopEnemy = false;			//���̓G���|�b�v�����邩�ǂ���
	int m_popedNum = 0;							//���݃|�b�v���Ă���G�̐�
	int m_killedNum = 0;						//�|���ꂽ�G�̐�
public:
	/**
	 * @brief �C���X�^���X���쐬����
	*/
	static void CreateInstance()
	{
		instance = new EnemyRepopManager;
	}

	/**
	 * @brief �C���X�^���X���擾����
	 * @return ���̃C���X�^���X
	*/
	static EnemyRepopManager& GetInstance()
	{
		static EnemyRepopManager instance;
		return instance;
	}

	/**
	 * @brief �C���X�^���X���폜����
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief �|�b�v�������G�̐���ݒ肷��
	 * @param state �|�b�v�������G�̐�
	*/
	void SetRepopEnemy(bool state)
	{
		m_shouldRepopEnemy = state;
	}

	/**
	 * @brief �G�����|�b�v������ׂ����ǂ������肷��
	 * @return ���|�b�v������ׂ����ǂ���
	*/
	bool ShouldRepopEnemy()
	{
		//�|�b�v�������G��0�łȂ��A�|�b�v�������G�����ׂē|����Ă�����
		if (m_popedNum != 0
			&& m_popedNum == m_killedNum)
		{
			//�G�̏������Z�b�g����
			m_popedNum = 0;
			m_killedNum = 0;
			return true;
		}
		return false;
	}

	/**
	 * @brief �|�b�v�������G�̐���ݒ肷��
	 * @param num �|�b�v�������G�̐�
	*/
	void SetPopedEnemyNum(int num)
	{
		m_popedNum = num;
	}

	/**
	 * @brief �|���ꂽ�G�̐���������
	*/
	void AddKilledEnemyNum()
	{
		m_killedNum++;
	}

	/**
	 * @brief �S�Ẵp�����[�^�[�����Z�b�g����
	*/
	void ResetParam()
	{
		m_shouldRepopEnemy = false;
		m_popedNum = 0;
		m_killedNum = 0;
	}
};

