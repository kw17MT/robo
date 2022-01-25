#pragma once
class SpriteRender;
class Fade;

/**
 * @brief �Q�[���I�[�o�[���o��A���g���C���^�C�g���ɖ߂邩�̈ē�������V�[��
*/
class AfterGameOverScene : public IGameObject
{
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_scale = Vector3::One;

	bool m_shouldStartNextScene = false;
	float m_alpha = 0.0f;

	enum EnSelectedSpriteTypes
	{
		enRePlayButton,
		enToTitleButton,
		enSpriteNum
	};

	EnSelectedSpriteTypes m_selectedSpriteType = enRePlayButton;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	AfterGameOverScene() {}

	/**
	 * @brief �f�X�g���N�^
	*/
	~AfterGameOverScene();

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
	 * @brief �㉺�{�^���ŉ�ʏ�{�^����I������֐�
	*/
	void SelectButton();
private:
	SpriteRender* m_sprite[enSpriteNum] = { nullptr };				//�摜�C���X�^���X�p�z��
	Fade* m_fade = nullptr;											//�t�F�[�h�C���X�^���X

	SpriteRender* m_blackSprite = nullptr;
};

