#pragma once
class SkinModelRender;
class Enemy;
class Effect;
class MissileBallistic;

#include "Weapon.h"

enum EnMoveStage
{
	enDeploying,
	enChaseTarget,
	enPassingBy
};

class Missile : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;							//�~�T�C���̈ʒu���W
	Vector3 m_deployDirection = Vector3::Zero;					//�W�J�������
	Vector3 m_moveSpeed = Vector3::Zero;						//�v�Z�ς݂̈ړ����x
	Vector3 m_moveDirection = g_camera3D->GetForward();			//���ړ��������
	Quaternion m_rot = Quaternion::Identity;					//�~�T�C���̉�]
	Vector3 m_prevMoveDirection = Vector3::Zero;				//1�t���[���O�̈ړ�����
	float m_lifeSpan = 0.0f;									//����
	bool m_firstCalc = false;									//�ŏ��̑��x�v�Z���ł�����
	float m_currentMoveSpeedRate = 0.0f;						//�ő呬�x�̉����o���Ă��邩

public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Missile() {}

	/**
	* @brief �f�X�g���N�^
	*/
	~Missile();

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
	 * @brief ���˒n�_���璅�e�\��n�_�̃x�N�g�������߂�
	 * @return ���˒n�_���璅�e�\��n�_�̒P�ʃx�N�g��
	*/
	Vector3 CalcToTargetVec();

	/**
	 * @brief �������ˈʒu��ݒ肷��
	 * @param pos 
	*/
	void SetLaunchedPos(Vector3 pos) { m_position = pos; }

	/**
	 * @brief �������^�[�Q�b�g����G�̃C���X�^���X��ݒ肷��
	 * @param enemy 
	*/
	void SetEnemy(Enemy* enemy) { m_enemy = enemy; }

	/**
	 * @brief �ǔ��̗}�����s��
	*/
	void RestrictRotation();

	//�~�T�C���̔��ˎ��̂݃����_���ȕ����ɓW�J
	Vector3 CalcDeployDirection();
private:
	SkinModelRender* m_skinModelRender = nullptr;				//���f���C���X�^���X
	Enemy* m_enemy = nullptr;									//�ڕW�Ƃ���G�̃C���X�^���X
	EnMoveStage m_moveStage = enDeploying;						//�ړ��^�C�v
	//MissileBallistic* m_ballistic = nullptr;					//�e���v�Z�C���X�^���X
};

