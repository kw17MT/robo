#pragma once

/**
 * @brief �V���h�E�쐬�N���X
*/
class Shadow
{
public:
	/**
	 * @brief �V���h�E�̃����_�����O�^�[�Q�b�g��������
	*/
	void InitShadowTarget();

	/**
	 * @brief �I�t�X�N���[���ɉe���쐬����B
	 * @param rc �����_�[�R���e�L�X�g
	*/
	void Render(RenderContext& rc);

	/**
	 * @brief �쐬�����V���h�E�}�b�v��Ԃ�
	 * @return �V���h�E�}�b�v
	*/
	RenderTarget& GetShadowMap()
	{
		return m_shadow;
	}
private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//����������F
	RenderTarget m_shadow;										//�e���������ރ^�[�Q�b�g
};

