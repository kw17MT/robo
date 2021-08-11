#pragma once
class FXAA
{
public:
	/**
	 * @brief AA作成の初期化を行う
	 * @param rt AAするレンダリングターゲット
	*/
	void Init(RenderTarget& rt);

	/**
	 * @brief アンチエイリアスする
	 * @param rc レンダーコンテキスト
	 * @param TargetToApply 適用したいターゲット
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	struct fxaa_data
	{
		float s_width;
		float s_height;
	};

	RenderTarget m_fxaaTarget;		//AAを行うターゲット
	Sprite m_finalSprite;				//適用した画像
	fxaa_data s_data;
	
};

