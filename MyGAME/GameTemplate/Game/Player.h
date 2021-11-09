#pragma once
class SkinModelRender;
class MachinGun;
class MissileGenerator;
class Reticle;
#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"

namespace
{
	const int animNum = 7;
}

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = { 0.0f,400.0f,5000.0f };			//�v���C���[�̌��݂̈ʒu
	Vector3 m_prevHomePosition = Vector3::Zero;			//1�t���[���O�̃z�[���|�W�V�����̈ʒu
	Vector3 m_currentHomePosition = Vector3::Zero;				//�J�������ǂ�������ʒu���W

public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Player() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~Player();

	/**
	 * @brief �C���X�^���X�������Ɉ�x�����Ă΂��֐�
	 * @return 
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief �v���C���[�̌��݈ʒu���擾����
	 * @return �v���C���[�̌��݈ʒu
	*/
	Vector3 GetPosition() { return m_currentHomePosition; }

	Reticle* GetReticleInstance() { return m_reticle; }
private:
	SkinModelRender* m_skinModelRender = nullptr;		//�v���C���[�̃��f���C���X�^���X
	MachinGun* m_machingun = nullptr;					//�v���C���[�����}�V���K���C���X�^���X
	MissileGenerator* m_missileGene = nullptr;
	Reticle* m_reticle = nullptr;

	CameraMove m_cameraMove;							//�v���C���[��Ǐ]����J�����̈ʒu���W�v�Z�I�u�W�F�N�g
	PlayerMove m_roboMove;								//�v���C���[�̍��W���v�Z����
	PlayerRotation m_roboRotation;						//�v���C���[�̉�]���v�Z����

	PlayerAnimation m_playerAnim;						//�v���C���[�̃A�j���[�V�����������I�u�W�F�N�g
	AnimationClip m_animClip[animNum];					//�v���C���[�̃A�j���[�V����
};

