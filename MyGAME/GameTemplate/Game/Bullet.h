#pragma once
class SkinModelRender;
class Effect;

enum EnOwner
{
	enPlayer,
	enEnemy
};

class Bullet : public IGameObject
{
private:
	Vector3 m_targetPos = Vector3::Zero;	//���e����ڕW�n�_
	Vector3 m_position = Vector3::Zero;		//���˂����ʒu
	Vector3 m_moveSpeed = Vector3::Zero;	//�ړ����x
	Quaternion m_rot = Quaternion::Identity;
	float count = 0;						//����
	bool m_firstCalc = false;				//�ŏ��̑��x�v�Z���ł�����
	EnOwner m_owner = enPlayer;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Bullet() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~Bullet();

	/**
	 * @brief �I�u�W�F�N�g�������Ɉ�x�����Ă΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[���ĂԊ֐�
	*/
	void Update();

	/**
	 * @brief ���˒n�_���璅�e�\��n�_�̃x�N�g�������߂�
	 * @return ���˒n�_���璅�e�\��n�_�̒P�ʃx�N�g��
	*/
	Vector3 CalcToTargetVec();

	/**
	 * @brief ���e�n�_�Ɣ��˒n�_��ݒ肷��
	 * @param target ���e�n�_
	 * @param pos ���˒n�_
	*/
	void SetTargetAndCurrentPos(Vector3 target, Vector3 pos)
	{
		m_targetPos = target;
		m_position = pos;
	}

	void SetOwner(EnOwner owner) { m_owner = owner; }

	void BehaveChangedByOwner();

	void SetRotation(Quaternion rot);

private:
	SkinModelRender* m_skinModelRender = nullptr;	//�e���f���̃I�u�W�F�N�g
	Effect* m_effect = nullptr;
};

