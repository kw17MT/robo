#pragma once
class Bullet;
class SkinModelRender;
class AmmoGauge;

#include "Weapon.h"

class MachinGun : public Weapon
{
private:
	Vector3 m_position = Vector3::Zero;			//�}�V���K���̈ʒu
	Vector3 m_targetPos = Vector3::Zero;		//���e������^�[�Q�b�g���W
	Vector3 m_playerPos = Vector3::Zero;
	Vector3 m_armDirection = Vector3::Zero;
	Quaternion m_qRot = Quaternion::Identity;
	int m_remaining_bullet = 100;
	float m_shootDelay = 0.15f;
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
	void SetTargetPos(Vector3 targetPos) /*override;*/
	{
		m_targetPos = targetPos;
	}

	void SetPosition(Vector3 handPos, Vector3 playerPos, Vector3 armDirection)
	{
		m_position = handPos;
		m_playerPos = playerPos;
		m_armDirection = armDirection;
	}

private:
	SkinModelRender* m_skinModelRender = nullptr;	//�}�V���K���̃��f��
	std::vector<Bullet*> m_bullets = { nullptr };	//���˂����e�̃C���X�^���X
	AmmoGauge* m_ammoGauge = nullptr;
};

