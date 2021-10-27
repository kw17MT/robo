#pragma once
class Bullet;
class SkinModelRender;

class MachinGun : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//�}�V���K���̈ʒu
	Vector3 m_targetPos = Vector3::Zero;		//���e������^�[�Q�b�g���W
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	MachinGun() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~MachinGun();

	/**
	 * @brief �I�u�W�F�N�g�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	virtual bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief ���e�n�_�Ɣ��˒n�_��ݒ肷��B
	 * @param targetPos 
	 * @param pos 
	*/
	void SetTargetAndCurrentPos(Vector3 targetPos, Vector3 pos)
	{
		m_position = pos;
		m_targetPos = targetPos;
	}
private:
	SkinModelRender* m_skinModelRender = nullptr;	//�}�V���K���̃��f��
	std::vector<Bullet*> m_bullets = { nullptr };	//���˂����e�̃C���X�^���X
};

