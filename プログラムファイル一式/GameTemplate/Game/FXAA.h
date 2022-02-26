#pragma once
class FXAA
{
public:
	/**
	 * @brief AA�쐬�̏��������s��
	 * @param rt AA���郌���_�����O�^�[�Q�b�g
	*/
	void Init(RenderTarget& rt);

	/**
	 * @brief �A���`�G�C���A�X����
	 * @param rc �����_�[�R���e�L�X�g
	 * @param TargetToApply �K�p�������^�[�Q�b�g
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	struct fxaa_data
	{
		float s_width;
		float s_height;
	};

	RenderTarget m_fxaaTarget;		//AA���s���^�[�Q�b�g
	Sprite m_finalSprite;				//�K�p�����摜
	fxaa_data s_data;
};

