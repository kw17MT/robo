#pragma once
class SpriteRender;

/**
 * @brief �t�F�[�h�̒i�K
*/
enum EnFadePhase
{
	enFadeIn,
	enFadeOut,
	enWait
};

/**
 * @brief �e�V�[���̑J�ڊԂőg�ݍ��ރt�F�[�h�N���X
*/
class Fade : public IGameObject
{
private:
	EnFadePhase m_fadePhase = enFadeIn;
	float m_alpha = 0.0f;
public:
	/**
	 * @brief �R���X�g���N�^
	*/
	Fade() {}

	/**
	 * @brief�@�f�X�g���N�^
	*/
	~Fade();

	/**
	 * @brief �C���X�^���X��������x�����Ă΂��֐�
	 * @return 
	*/
	bool Start();

	/**
	 * @brief ���t���[���Ă΂��֐�
	*/
	void Update();

	/**
	 * @brief ���݂̃t�F�[�h�̒i�K���擾����
	 * @return ���݂̃t�F�[�h�̒i�K
	*/
	EnFadePhase& GetFadePhase() { return m_fadePhase; }

	/**
	 * @brief �t�F�[�h�A�E�g����悤�ɐݒ肷��
	*/
	void SetFadeOut() { m_fadePhase = enFadeOut; }
private:
	SpriteRender* m_sprite = nullptr;
};

