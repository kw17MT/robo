#pragma once
class Bullet;
class SkinModelRender;

class EnemyMachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//�}�V���K���̈ʒu
	Vector3 m_targetPos = Vector3::Zero;		//���e������^�[�Q�b�g���W
	bool m_canShoot = false;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	EnemyMachinGun() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~EnemyMachinGun();

	/**
	 * @brief �I�u�W�F�N�g�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief ���e�n�_�Ɣ��˒n�_��ݒ肷��B
	 * @param targetPos
	 * @param pos
	*/
	void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos) /*override;*/
	{
		m_position = pos;
		m_targetPos = targetPos;
	}

	void SetCanShoot(bool canShoot) { m_canShoot = canShoot; }

private:
	SkinModelRender* m_skinModelRender = nullptr;	//�}�V���K���̃��f��
	std::vector<Bullet*> m_bullets = { nullptr };	//���˂����e�̃C���X�^���X
};

