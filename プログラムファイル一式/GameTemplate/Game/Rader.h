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
	 * @brief �w��z��ԍ��ɓG�̈ʒu�Ɛ�����Ԃ�ۑ�����
	 * @param no �w��v�f�ԍ�
	 * @param enemyPos �G�̈ʒu
	*/
	void SaveEnemyPosAndIsAlive(int no, Vector4 enemyPos);

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
	Vector4 m_enemyPosAndIsAlive[10];							//�G�̈ʒu�Ɛ�����
	Vector3 m_prevEnemyPos[10];									//1�t���[���O�̓G�̈ʒu
	EnemyIconOnRader* m_enemyIcon[10] = { nullptr };			//���[�_�[�̓G
	SpriteRender* m_playerIcon = nullptr;						//�v���C���[
	SpriteRender* m_rader = nullptr;
};

