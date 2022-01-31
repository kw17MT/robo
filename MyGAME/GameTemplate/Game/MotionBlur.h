#pragma once
class MotionBlur
{
public:
	/**
	 * @brief ���[�V�����u���[���쐬�Ɏg���X�v���C�g�̏�����
	 * @param defferedTarget �ʏ�Q�[�����
	 * @param normalTarget �@���}�b�v
	 * @param velocityTarget �x���V�e�B�}�b�v
	*/
	void InitSprite(RenderTarget& defferedTarget, RenderTarget& normalTarget, RenderTarget& velocityTarget);

	/**
	 * @brief ���[�V�����u���[���쐬���ă��C����ʂɉ��Z��������
	 * @param rc �����_�[�R���e�L�X�g
	 * @param mainSprite ���C����ʂ̃X�v���C�g
	 * @param targetToApply ���[�V�����u���[��K�p���郁�C�����
	*/
	void Render(RenderContext& rc, Sprite& mainSprite , RenderTarget& targetToApply);

private:
	RenderTarget m_motionBlurTarget;		//���[�V�����u���[���쐬����^�[�Q�b�g
	Sprite m_motionBlurSprite;				//�쐬�������[�V�����u���[�摜
	GaussianBlur m_gaussian[4];				//�K�E�V�A���u���[
};

