#pragma once
class SkinModelRender;
class DisplayDistance;
class Player;
class EnemyStateIcon;
class EnemyBrain;
class MachinGun;

#include "EnemyHP.h"
#include "EnemyMove.h"
#include "EnemyBrain.h"

class Enemy : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;			//�ʒu���W
	Quaternion m_rot = Quaternion::Identity;	//��]
	float m_distance = 0;						//�G�ƃv���C���[�̋���		
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Enemy() {};

	/**
	 * @brief �f�X�g���N�^
	*/
	~Enemy();

	/**
	 * @brief �C���X�^���X���������ꂽ���Ɉ�x�����Ă΂��֐�
	 * @return true�Ő���I��
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief �V�����ʒu���W��ݒ肷��B
	 * @param pos �V�����ʒu
	*/
	void SetPosition(Vector3 pos) { m_position = pos; }

	/**
	 * @brief ���̓G�̈ʒu���W���擾����
	 * @return �G�̈ʒu���W
	*/
	Vector3 GetPosition() { return m_position; }

	/**
	 * @brief �_���[�W��HP�ɓK�p����
	 * @param damageType �_���[�W�̎��
	*/
	void TakenDamage(EnDamageTypes damageType);
private:
	SkinModelRender* m_skinModelRender = nullptr;			//�G�̃��f��
	DisplayDistance* m_displayDistance = nullptr;			//�v���C���[�ƓG�̋�����\��
	Player* m_player = nullptr;								//�v���C���[�̃C���X�^���X�擾�p
	EnemyStateIcon* m_enemyStateIcon = nullptr;				//�G�ɂ�����A�C�R���S�ʂ�\��

	EnemyBrain m_enemyBrain;								//�G�̍s�����܂Ƃ߂ĊǗ�����
	EnemyHP* m_enemyHP = nullptr;							//�G��HP��HP�o�[��\������
	//MachinGun* m_machinGun = nullptr;
};

