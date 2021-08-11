#pragma once
class DepthInView
{
public:
	/**
	 * @brief 被写界深度作成の初期化を行う
	 * @param rt 被写界深度をするレンダリングターゲット
	*/
	void Init(RenderTarget& rt);

	/**
	 * @brief 被写界深度の作成
	 * @param rc レンダーコンテキスト
	 * @param TargetToApply 適用したいターゲット
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	GaussianBlur m_depthGaussian;		//ブラー作成用
	RenderTarget m_depthInViewMap;		//被写界深度を適用する
	Sprite m_main;
	Sprite m_finalSprite;				//適用した画像

	RenderTarget* m_depthTargets[2];		//Init関数で初期化
};

