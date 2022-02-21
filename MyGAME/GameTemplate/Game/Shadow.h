#pragma once

/**
 * @brief シャドウ作成クラス
*/
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
	void Render(RenderContext& rc);

	/**
	 * @brief 作成したシャドウマップを返す
	 * @return シャドウマップ
	*/
	RenderTarget& GetShadowMap()
	{
		return m_shadow;
	}
private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//初期化する色
	RenderTarget m_shadow;										//影を書き込むターゲット
};

