#pragma once
class EnemyGenerator;
class Player;
class SkyCube;
class Ground;
class UI;
class Sun;

class Game : public IGameObject
{
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Game() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~Game();

	/**
	 * @brief �I�u�W�F�N�g���������ꂽ��x�����Ă΂��֐�
	 * @return 
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂�֐�
	*/
	void Update();
private:
	Player* m_player = nullptr;						//�v���C���[�C���X�^���X
	EnemyGenerator* m_enemyGenerator = nullptr;		//�G������C���X�^���X
	SkyCube* m_sky = nullptr;						//�X�J�C�L���[�u�}�b�v�C���X�^���X
	Ground* m_ground = nullptr;						//�n�ʃC���X�^���X
	UI* m_ui = nullptr;								//UI�C���X�^���X
	Sun* m_sun = nullptr;							//���z�C���X�^���X
};

