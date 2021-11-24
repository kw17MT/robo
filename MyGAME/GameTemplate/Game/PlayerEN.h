#pragma once
class SpriteRender;

class PlayerEN : public IGameObject
{
private:
	Vector3 m_screenPos = { 0.0f, -300.0f, 0.0f };				//�Œ���W

	float m_playerEN = 100.0f;									//�v���C���[�̍ő�G�l���M�[
	bool m_isPlayerEnRemain = true;								//�v���C���[�̃G�l���M�[�͎c���Ă��邩
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	PlayerEN() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~PlayerEN();

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
	 * @brief �v���C���[�̃G�l���M�[�͎c���Ă��邩�̊m�F�֐�
	 * @return TRUE�c���Ă���@FALSE�Ȃ�
	*/
	bool GetIsPlayerENRemain() { return m_isPlayerEnRemain; }
private:
	SpriteRender* m_spriteRender = nullptr;						//EN�\���p�摜
};

