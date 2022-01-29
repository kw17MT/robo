#pragma once
#include "SpriteRender.h"
#include "FontRender.h"

enum EnPlayerDamageTypes
{
	enEnemyBullet,		//�e�ۂɂ��_���[�W
	enEnemyMissile,		//�~�T�C���ɂ��_���[�W
	enEnemyRaser		//���[���K���ɂ��_���[�W
};

class PlayerHP : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -300.0f, 0.0f };			//�Œ�ʒu

	float m_playerHp = 100.0f;								//�v���C���[�̏���HP
	float m_hpSize = m_playerHp;
	float m_damagedSpriteAlpha = 0.0f;
	bool m_isPlayerAlive = true;							//�v���C���[�͐����Ă��邩
	bool m_isSoundBuzzer = false;
	bool m_isSoundDamagedSE1 = false;
	bool m_isSoundDamagedSE2 = false;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	PlayerHP() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~PlayerHP();

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
	 * @brief �v���C���[�������Ă��邩�̊m�F�֐�
	 * @return TRUE�����Ă���@FALSE�|����Ă���
	*/
	bool GetIsPlayerAlive() { return m_isPlayerAlive; }

	/**
	 * @brief �v���C���[�Ƀ_���[�W��K�p����
	 * @param enemyDamageType �_���[�W�̎��
	*/
	void ApplyDamage(EnPlayerDamageTypes enemyDamageType);

	/**
	 * @brief �v���C���[��HP����C�Ɍ��炷
	*/
	void SetHPZero() { m_playerHp = 0.0f; }

	void SoundDamagedSE(int soundNo);

	/**
	 * @brief HP��7����������Ԃ�MAX�Ƃ����Ƃ��̎c��HP�o�[�̑傫��
	 * @return �c��HP�T�C�Y / �댯��ԂƂ����
	*/
	float GetHPBarAmountInDanger();
private:
	SpriteRender* m_spriteRender = nullptr;				//HP�\���p�摜
	SpriteRender* m_damagedSprite = nullptr;			//�_���[�W���󂯂��Ƃ��̎���ɏo��摜
};

