#pragma once
class DefferedLighting
{
private:
public:
	/**
	 * @brief ディファードレンダリングに必要なレンダーターゲットの初期化
	*/
	void InitTargets();

	/**
	 * @brief 使用する画像の初期化
	 * @param shadowTarget 使用するシャドウマップ
	*/
	void InitSprite(RenderTarget& shadowTarget);

	/**
	 * @brief ディファードレンダリングを開始する
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc);

	void Draw(RenderContext& rc);
private:
	RenderTarget m_albedoTarget, m_normalTarget, m_specAndDepthTarget;										//作成するオフスクリーン
	RenderTarget* m_defferedTargets[3] = { &m_albedoTarget, &m_normalTarget, &m_specAndDepthTarget };		//MRT用	
	SpriteInitData m_defferedSpriteData;																	//ディファードレンダリング画像のパラメータ
	Sprite m_defferedSprite;																				//最終の画像
};

