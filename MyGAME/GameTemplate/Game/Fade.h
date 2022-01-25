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
	bool m_stopFadeInProgress = false;
	float m_stopAlpha = 1.0f;
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
	 * @brief ���݂̃t�F�[�h�œK�p���Ă���A���t�@�l���擾����
	 * @return ���݂̃A���t�@�l
	*/
	float GetFadeAlpha() const
	{
		return m_alpha;
	}

	/**
	 * @brief ���݂̃t�F�[�h�̒i�K���擾����
	 * @return ���݂̃t�F�[�h�̒i�K
	*/
	EnFadePhase& GetFadePhase() { return m_fadePhase; }

	/**
	 * @brief �t�F�[�h�A�E�g����悤�ɐݒ肷��
	*/
	void SetFadeOut() { m_fadePhase = enFadeOut; }

	/**
	 * @brief �t�F�[�h��r���Ŏ~�߂�
	 * @param alpha �~�߂����摜�̔Z��
	*/
	void SetStopFadeInProgress(bool isStopInProgress, float alpha = 1.0f);
private:
	SpriteRender* m_sprite = nullptr;
};

