#pragma once
class SpriteRender;
class Enemy;

class MissileTargetIcon : public IGameObject
{
private:
	Vector3 m_screenPos = Vector3::Zero;				//�X�N���[����ł̈ʒu
	Vector3 m_targetedEnemyPos = Vector3::Zero;			//�^�[�Q�b�g����Ă���G�̈ʒu
	Vector3 m_scale = Vector3::Zero;					//�g�嗦
	Quaternion qRot = Quaternion::Identity;				//��]

	float m_angle = 0.0f;								//�A�C�R���ɓK�p����p�x
	bool m_isFirstExpand = false;						//�ŏ��̊g������邩�ǂ���
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	MissileTargetIcon() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~MissileTargetIcon();

	/**
	 * @brief �C���X�^���X�������ɌĂ΂��֐�
	 * @return true
	*/
	bool Start();

	/**
	 * @brief ���t���[�����t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief �^�[�Q�b�g�ƂȂ�G�̈ʒu��ݒ�
	 * @param enemyPos �G�̈ʒu
	*/
	void SetTargetedEnemyPos(Vector3 enemyPos) { m_targetedEnemyPos = enemyPos; }

	/**
	 * @brief �^�[�Q�b�g�ƂȂ�G�̃C���X�^���X���擾
	 * @param enemy �G
	*/
	void SetTargetedEnemy(Enemy* enemy) { m_enemy = enemy; }

	/**
	 * @brief �^�[�Q�b�g�����ŏ��͊g�債����Ԃŏo��
	 * @param state �g�債����
	*/
	void SetFirstExpandScale(bool state) { m_isFirstExpand = state; }
private:
	SpriteRender* m_spriteRender = nullptr;		//�A�C�R���摜�̃C���X�^���X
	Enemy* m_enemy = nullptr;					//�A�C�R����\������G�̃C���X�^���X
};

