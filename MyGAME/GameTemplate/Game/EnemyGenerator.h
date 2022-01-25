#pragma once
class Enemy;
#include "EnemySpawnPoint.h"

class EnemyGenerator : public IGameObject
{
private:
	Vector3 m_firstEnemyPos = Vector3::Zero;			//��ԍŏ��ɏo���G�̈ʒu
	bool m_shouldGenerateEnemy = false;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	EnemyGenerator() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~EnemyGenerator();

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
	 * @brief ���������G�̐����擾����
	 * @return �G�̐�
	*/
	int GetEnemyNum() { return m_enemys.size() - 1; }

	/**
	 * @brief �G�̈ʒu���擾
	 * @param no �z��̔ԍ�
	 * @return �G�̈ʒu
	*/
	Vector3 GetEnemyPos(int no);

	/**
	 * @brief �w��z��ԍ��̓G�C���X�^���X�͐����Ă��邩
	 * @param no �w�肷��v�f�ԍ�
	 * @return TRUE�������Ă���@FALSE������ł���
	*/
	bool GetIsEnemyAlive(int no);

	/**
	 * @brief �G�C���X�^���X��ۑ�����z�������ɂ���
	*/
	void CleanUpArray();

	/**
	 * @brief �G�𐶐�����
	*/
	void GenerateEnemy();
private:
	std::vector<Enemy*> m_enemys = { nullptr };				//�G�C���X�^���X
	EnemySpawnPoint m_spawnPoint;							//�G�̃X�|�[���|�C���g�����肷��
};

