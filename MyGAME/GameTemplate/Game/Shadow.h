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
	void Render(RenderContext& rc);

	/**
	 * @brief 作成したシャドウマップを返す
	 * @return シャドウマップ
	*/
	RenderTarget& GetShadowTarget()
	{
		return m_shadowTarget;
	}
private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//画面を全部きれいにするときの色
	RenderTarget m_shadowTarget;								//影映し出すオフスクリーン
};

