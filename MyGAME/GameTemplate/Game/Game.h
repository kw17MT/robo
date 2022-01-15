#pragma once
class EnemyGenerator;
class Player;
class SkyCube;
class Ground;
class UI;
class Sun;
class Rader;
class LaunchPad;
class SCoundSource;
class EliminateTelop;
class ObjectiveEnemyNum;

class Game : public IGameObject
{
private:
	int m_killedEnemyNum = 0;
	bool m_isSoundClear = false;
	float m_colapsedTimeAfterClear = 0.0f;
	float m_bgmVolume = 0.5f;
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
	Rader* m_rader = nullptr;						//���[�_�[�C���X�^���X
	LaunchPad* m_launchPad = nullptr;
	CSoundSource* m_bgm = nullptr;
	EliminateTelop* m_eliminateTelop = nullptr;
	ObjectiveEnemyNum* m_objectiveEnemyNum = nullptr;
};

