#pragma once
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
	RenderTarget& GetShadowTarget()
	{
		return m_shadowTarget;
	}
private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//��ʂ�S�����ꂢ�ɂ���Ƃ��̐F
	RenderTarget m_shadowTarget;								//�e�f���o���I�t�X�N���[��
};

