#pragma once
class SkinModelRender;
class MachinGun;
class MissileGenerator;
class Reticle;
class PlayerEN;
class RestrictArea;
class PlayerEffect;

#include "CameraMove.h"
#include "PlayerMove.h"
#include "PlayerRotation.h"
#include "PlayerAnimation.h"
#include "PlayerHP.h"

#include "effect/Effect.h"

namespace
{
	const int animNum = 7;
}

enum EnDeathTypes
{
	enStillAlive,
	enByEnemy,
	enAwayFromArea
};

class Player : public IGameObject
{
private:
	Vector3 m_currentPosition = { 0.0f,295.0f,5000.0f };			//�v���C���[�̌��݂̈ʒu
	Vector3 m_prevHomePosition = Vector3::Zero;			//1�t���[���O�̃z�[���|�W�V�����̈ʒu
	Vector3 m_currentHomePosition = Vector3::Zero;				//�J�������ǂ�������ʒu���W

	EnDeathTypes m_deathType = enStillAlive;

	Quaternion m_effectRot = Quaternion::Identity;

	int a = 0;
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
	 * @brief �v���C���[�̌��݂̃z�[���|�W�V�������擾����
	 * @return �v���C���[�̌��݈ʒu
	*/
	Vector3 GetHomePosition() { return m_currentHomePosition; }

	Vector3 GetRoboPosition() { return m_currentPosition; }

	void TakenDamage(EnPlayerDamageTypes damageType);

	Reticle* GetReticleInstance() { return m_reticle; }

	void SetPlayerPosForLaunch(Vector3 pos) { m_skinModelRender->SetPosition(pos); }
	Vector3 GetPlayerPosForLaunch() { return m_skinModelRender->GetPosition(); }
private:
	SkinModelRender* m_skinModelRender = nullptr;		//�v���C���[�̃��f���C���X�^���X
	MachinGun* m_machingun = nullptr;					//�v���C���[�����}�V���K���C���X�^���X
	MissileGenerator* m_missileGene = nullptr;
	Reticle* m_reticle = nullptr;
	PlayerHP* m_playerHp = nullptr;
	PlayerEN* m_playerEn = nullptr;
	RestrictArea* m_area = nullptr;
	PlayerEffect* m_playerEffect = nullptr;

	CameraMove m_cameraMove;							//�v���C���[��Ǐ]����J�����̈ʒu���W�v�Z�I�u�W�F�N�g
	PlayerMove m_roboMove;								//�v���C���[�̍��W���v�Z����
	PlayerRotation m_roboRotation;						//�v���C���[�̉�]���v�Z����

	PlayerAnimation m_playerAnim;						//�v���C���[�̃A�j���[�V�����������I�u�W�F�N�g
	AnimationClip m_animClip[animNum];					//�v���C���[�̃A�j���[�V����
};

