#pragma once
class GaussianBlur;

class LensGhost
{
public:
	/**
	 * @brief レンダリングターゲットとスプライトの初期化
	*/
	void Init();
	
	/**
	 * @brief 太陽をオフスクリーンレンダリングし、ブラーを掛けてゴーストを作る
	 * @param rc レンダーコンテキスト
	 * @param target 加算対象のレンダリングターゲット
	*/
	void Render(RenderContext& rc, RenderTarget& target);

private:
	RenderTarget m_ghostTarget;
	Sprite m_lensGhost;
	GaussianBlur m_gaussianBlur;
};

