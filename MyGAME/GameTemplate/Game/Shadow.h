#pragma once
class Shadow
{
public:
	/**
	 * @brief シャドウのレンダリングターゲットを初期化
	*/
	void InitShadowTarget();

	/**
	 * @brief オフスクリーンに影を作成する。
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc, RenderTarget& shadowMap);

	
private:
};

