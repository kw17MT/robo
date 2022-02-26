#pragma once
class SkinModelRender;

class EnemyMachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//�}�V���K���̈ʒu
	Vector3 m_targetPos = Vector3::Zero;		//���e������^�[�Q�b�g���W
	bool m_canShoot = false;					//���ݒe�����Ă邩�ǂ���
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
	 * @brief ���e�n�_��ݒ肷��B
	 * @param targetPos
	*/
	void SetTargetPos(Vector3 targetPos) { m_targetPos = targetPos; }

	/**
	 * @brief ���������˂��ꂽ�ʒu��ݒ肷��
	 * @param pos ���˂��ꂽ�ʒu
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief ���˂ł��邩�ǂ�����ݒ肷��
	 * @param canShoot ���˂ł��邩�ǂ���
	*/
	void SetCanShoot(bool canShoot) { m_canShoot = canShoot; }

private:
	SkinModelRender* m_skinModelRender = nullptr;	//�}�V���K���̃��f��
};

