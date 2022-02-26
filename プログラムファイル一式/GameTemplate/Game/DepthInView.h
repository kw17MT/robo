#pragma once
class DepthInView
{
public:
	/**
	 * @brief 被写界深度作成の初期化を行う
	 * @param rt 被写界深度をするレンダリングターゲット
	*/
	void Init(RenderTarget& mainTarget, RenderTarget& depthTarget);

	/**
	 * @brief 被写界深度の作成
	 * @param rc レンダーコンテキスト
	 * @param TargetToApply 適用したいターゲット
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	GaussianBlur m_depthGaussian;		//ブラー作成用
	Sprite m_finalSprite;				//適用した画像
};

