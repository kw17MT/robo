#pragma once
class Bloom
{
public:
	/**
	 * @brief �u���[���쐬�̏��������s��
	 * @param rt �P�x���o�����������_�����O�^�[�Q�b�g
	*/
	void Init(RenderTarget& rt);

	/**
	 * @brief �쐣���u���[�����쐬���^�[�Q�b�g�ɕ`��
	 * @param rc �����_�[�R���e�L�X�g
	 * @param TargetToApply �u���[����K�p�������^�[�Q�b�g
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	RenderTarget m_luminanceTarget;		//�P�x�擾�p�^�[�Q�b�g
	GaussianBlur m_gaussianBlur[4];		//�쐣���K�E�V�A���u���[�쐬�p�����o
	Sprite m_bloomSprite;				//�u���[���摜
	Sprite m_finalSprite;				//�u���[����K�p�����摜
};