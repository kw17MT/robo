#pragma once
class SpriteRender;

//�^����ꂽ�_���[�W�̎��
enum EnDamageTypes
{
	enBullet,
	enMissile,
	enRaser
};

class EnemyHP : public IGameObject
{
private:
	Vector3 m_enemyPos = Vector3::Zero;		//�G�̈ʒu
	Vector3 m_screenPos = Vector3::Zero;	//�X�N���[�����W
	Vector3 m_scale = Vector3::One;			//�g�嗦

	int m_enemyHP = 100;					//�G��HP
	bool m_isTargeted = false;				//�^�[�Q�b�g����Ă��鎞����HP�o�[������
	bool m_isEnemyDead = false;					//���̓G�͓|���ꂽ��
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	EnemyHP() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~EnemyHP();

	/**
	 * @brief �C���X�^���X���������ꂽ���Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief ���̓G�̓^�[�Q�b�g����Ă��邩
	 * @param isTargeted ���݃^�[�Q�b�g����Ă��邩
	*/
	void IsEnemyTargeted(bool isTargeted) { m_isTargeted = isTargeted; }

	/**
	 * @brief �Ǐ]����G�̈ʒu�̍��W
	 * @param enemyPos �G�̈ʒu
	*/
	void SetEnemyPos(Vector3 enemyPos) { m_enemyPos = enemyPos; }

	/**
	 * @brief �_���[�W��K�p����
	 * @param damageType �_���[�W�^�C�v
	*/
	void ApplyDamage(EnDamageTypes damageType);

	/**
	 * @brief �|���ꂽ��
	 * @return TRUE���|���ꂽ�AFALSE�������Ă���
	*/
	bool IsEnemyDead() {return m_isEnemyDead;}
private:
	SpriteRender* m_spriteRender = nullptr;			//HP�o�[�̃C���X�^���X
};

