#pragma once
class DepthInView
{
public:
	/**
	 * @brief ��ʊE�[�x�쐬�̏��������s��
	 * @param rt ��ʊE�[�x�����郌���_�����O�^�[�Q�b�g
	*/
	void Init(RenderTarget& mainTarget, RenderTarget& depthTarget);

	/**
	 * @brief ��ʊE�[�x�̍쐬
	 * @param rc �����_�[�R���e�L�X�g
	 * @param TargetToApply �K�p�������^�[�Q�b�g
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	GaussianBlur m_depthGaussian;		//�u���[�쐬�p
	RenderTarget m_depthInViewMap;		//��ʊE�[�x��K�p����
	Sprite m_main;
	Sprite m_finalSprite;				//�K�p�����摜

	RenderTarget* m_depthTargets[2];		//Init�֐��ŏ�����
};

