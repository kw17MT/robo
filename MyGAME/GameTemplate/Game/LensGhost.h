#pragma once
class GaussianBlur;

class LensGhost
{
public:
	/**
	 * @brief �����_�����O�^�[�Q�b�g�ƃX�v���C�g�̏�����
	*/
	void Init();
	
	/**
	 * @brief ���z���I�t�X�N���[�������_�����O���A�u���[���|���ăS�[�X�g�����
	 * @param rc �����_�[�R���e�L�X�g
	 * @param target ���Z�Ώۂ̃����_�����O�^�[�Q�b�g
	*/
	void Render(RenderContext& rc, RenderTarget& target);

private:
	RenderTarget m_ghostTarget;
	Sprite m_lensGhost;
	GaussianBlur m_gaussianBlur;
};

