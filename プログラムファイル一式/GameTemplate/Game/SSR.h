#pragma once
class SSR
{
public:
	/**
	 * @brief SSR�쐬�̏��������s��
	 * @param rt SSR���郌���_�����O�^�[�Q�b�g
	*/
	void Init(RenderTarget& mainRenderTarget,
		RenderTarget& depthRenderTarget,
		RenderTarget& normalRenderTarget,
		RenderTarget& metallicSmoothRenderTarget,
		RenderTarget& albedoRenderTarget);

	/**
	 * @brief SSR���������摜���쐬����
	 * @param rc �����_�[�R���e�L�X�g
	 * @param TargetToApply �K�p�������^�[�Q�b�g
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	struct Ssr_Cb
	{
		Matrix s_viewProjectionInv;
		Matrix s_viewProjection;
		Vector3 cameraPosInWorld;
		int s_rayNum = 10;
		float s_RayDistance = 200.0f;
	};

	RenderTarget m_reflectionTarget;
	RenderTarget m_finalTarget;
	GaussianBlur m_blur;
	Sprite m_reflectionSprite;
	Sprite m_finalSprite;

	Ssr_Cb m_ssrData;
};

