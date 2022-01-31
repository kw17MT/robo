#pragma once

/**
 * @brief �X�e�[�W�̎��
*/
enum EnStage
{
	en1_1,
	en1_2,
	enStageNum
};

/**
 * @brief �X�e�[�W�̃}�l�[�W���[
 * ���݂͖��g�p
*/
class StageManager : public IGameObject
{
private:
	static StageManager* instance;
	EnStage m_stage = en1_1;
public:
	StageManager() {};
	/**
	 * @brief �C���X�^���X�����
	*/
	static void CreateInstance()
	{
		instance = new StageManager;
	}

	/**
	 * @brief �C���X�^���X���擾����
	 * @return �V���O���g���C���X�^���X
	*/
	static StageManager& GetInstance()
	{
		static StageManager instance;
		return instance;
	}

	/**
	 * @brief �C���X�^���X���폜����B
	*/
	static void DeleteInstance()
	{
		delete instance;
	}

	/**
	 * @brief ���݂̃X�e�[�W���擾����
	 * @return ���݂̃X�e�[�W�ԍ�
	*/
	EnStage GetStage() const
	{
		return m_stage;
	}

	/**
	 * @brief �g�p����X�e�[�W��ݒ肷��
	 * @param stage �g�p����X�e�[�W�ԍ�
	*/
	void SetStage(EnStage stage)
	{
		m_stage = stage;
	}
};

