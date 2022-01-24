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
class ClearTelop;
class Fade;
class AfterGameOverScene;

/**
 * @brief �C���Q�[���N���X
*/
class Game : public IGameObject
{
private:
	int m_killedEnemyNum = 0;					//���������G�̑���
	bool m_isSoundClear = false;				//�N���A�T�E���h���Ȃ炵�����ǂ���
	float m_colapsedTimeAfterClear = 0.0f;		//�N���A������̌o�ߎ���
	float m_bgmVolume = 0.5f;					//BGM�̉���
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
	Player* m_player = nullptr;							//�v���C���[�C���X�^���X
	EnemyGenerator* m_enemyGenerator = nullptr;			//�G������C���X�^���X
	SkyCube* m_sky = nullptr;							//�X�J�C�L���[�u�}�b�v�C���X�^���X
	Ground* m_ground = nullptr;							//�n�ʃC���X�^���X
	UI* m_ui = nullptr;									//UI�C���X�^���X
	Sun* m_sun = nullptr;								//���z�C���X�^���X
	Rader* m_rader = nullptr;							//���[�_�[�C���X�^���X
	CSoundSource* m_bgm = nullptr;						//BGM�C���X�^���X
	EliminateTelop* m_eliminateTelop = nullptr;			//�r�ł���e���b�v
	ObjectiveEnemyNum* m_objectiveEnemyNum = nullptr;	//�ڕW���j���\���C���X�^���X
	ClearTelop* m_clear = nullptr;						//�N���A�e���b�v�C���X�^���X
	Fade* m_fade = nullptr;								//�t�F�[�h�C���X�^���X
	AfterGameOverScene* m_afterGOScene = nullptr;		//�Q�[���I�[�o�[��̊e�V�[���ւ̑J�ڗU���C���X�^���X
};	

