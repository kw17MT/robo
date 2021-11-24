#pragma once
class SpriteRender;
class EnemyIconOnRader;

class Rader : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;		//�X�N���[����̈ʒu
	Vector3 m_playerPos = Vector3::Zero;		//�v���C���[�̈ʒu
	Vector3 m_scale = Vector3::Zero;			//�g�嗦
	Vector2 m_pivot = Vector2::Zero;			//����

	int m_enemyNum = 0;							//�G�̐�
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Rader() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~Rader();

	/**
	 * @brief �C���X�^���X�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief �w��̗v�f�ԍ��ɓG�̈ʒu���W��ۑ�����
	 * @param no �v�f�ԍ�
	 * @param enemyPos �G�̈ʒu���W
	*/
	void SaveEnemyPos(int no, Vector3 enemyPos);

	/**
	 * @brief ��������Ă���G�̐���ۑ�����
	 * @param num �G�̐�
	*/
	void SetEnemyNum(int num) { m_enemyNum = num; }

	/**
	 * @brief �v���C���[�̍��W��ۑ�����
	 * @param pos �v���C���[�̍��W
	*/
	void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
private:
	Vector3 m_enemyPos[10];										//�G�̈ʒu
	Vector3 m_prevEnemyPos[10];
	EnemyIconOnRader* m_enemyIcon[10] = { nullptr };				//���[�_�[�̓G
	SpriteRender* m_playerIcon = nullptr;						//�v���C���[
	SpriteRender* m_rader = nullptr;
};

