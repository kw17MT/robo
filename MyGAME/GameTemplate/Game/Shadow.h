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
	void Render(RenderContext& rc, RenderTarget& shadowMap);

	
private:
};

