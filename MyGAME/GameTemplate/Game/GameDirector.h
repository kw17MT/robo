#pragma once

/**
 * @brief �Q�[���V�[��
*/
enum EnGameScene
{
	enTitle,				//�^�C�g��
	enExit,					//�Q�[���I��
	enLaunchingPlayer,		//�v���C���[�o�����o��
	enInGame,				//�Q�[����
	enGameClear,			//�Q�[���N���A�V�[����
	enGameOver,				//�Q�[���I�[�o�[���o
	enAfterGameOver,		//�Q�[���I�[�o�[���o�I����
	enToTitle				//�^�C�g���֖߂�
};

class GameDirector
{
private:
	static GameDirector* instance;				//�V���O���g���C���X�^���X
	EnGameScene enGameScene = enTitle;			//���݂̃Q�[���V�[��
public:
	/**
	 * @brief �C���X�^���X���쐬����
	*/
	static void CreateInstance()
	{
		instance = new GameDirector;
	}

	/**
	 * @brief �C���X�^���X���擾����
	 * @return ���̃C���X�^���X
	*/
	static GameDirector& GetInstance()
	{
		static GameDirector instance;
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
	 * @brief ���݂̃Q�[���V�[�����擾����
	 * @return �Q�[���V�[��
	*/
	EnGameScene GetGameScene()
	{
		return enGameScene;
	}

	/**
	 * @brief ���݂̃Q�[���V�[����ݒ肷��
	 * @param scene ���݂̃Q�[���V�[��
	*/
	void SetGameScene(EnGameScene scene)
	{
		enGameScene = scene;
	}
};

