#pragma once
class SSR
{
public:
	/**
	 * @brief SSR作成の初期化を行う
	 * @param rt SSRするレンダリングターゲット
	*/
	void Init(RenderTarget& mainRenderTarget,
		RenderTarget& depthRenderTarget,
		RenderTarget& normalRenderTarget,
		RenderTarget& metallicSmoothRenderTarget,
		RenderTarget& albedoRenderTarget);

	/**
	 * @brief SSRした合成画像を作成する
	 * @param rc レンダーコンテキスト
	 * @param TargetToApply 適用したいターゲット
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

