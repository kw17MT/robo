#pragma once
class DefferedLighting
{
private:
public:
	/**
	 * @brief �f�B�t�@�[�h�����_�����O�ɕK�v�ȃ����_�[�^�[�Q�b�g�̏�����
	*/
	void InitTargets();

	/**
	 * @brief �g�p����摜�̏�����
	 * @param shadowTarget �g�p����V���h�E�}�b�v
	*/
	void InitSprite(RenderTarget& shadowTarget);

	/**
	 * @brief �f�B�t�@�[�h�����_�����O���J�n����
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Render(RenderContext& rc);

	void Draw(RenderContext& rc);
private:
	RenderTarget m_albedoTarget, m_normalTarget, m_specAndDepthTarget;										//�쐬����I�t�X�N���[��
	RenderTarget* m_defferedTargets[3] = { &m_albedoTarget, &m_normalTarget, &m_specAndDepthTarget };		//MRT�p	
	SpriteInitData m_defferedSpriteData;																	//�f�B�t�@�[�h�����_�����O�摜�̃p�����[�^
	Sprite m_defferedSprite;																				//�ŏI�̉摜
};

