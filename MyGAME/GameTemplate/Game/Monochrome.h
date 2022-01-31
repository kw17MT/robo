#pragma once

class Monochrome
{
private:
	float m_monochromeRate = 0.0f;			//モノクロ度
public:
	/**
	 * @brief モノクロ化させるターゲットとモノクロ化できたスプライトの初期化
	*/
	void Init();

	/**
	 * @brief モノクロ化させたいメイン画面をコピーし、モノクロ化した画像を描画する
	 * @param rc レンダーコンテキスト
	 * @param mainSprite メインのゲーム画面
	 * @param mainTarget メインのゲーム画面が使うレンダーターゲット
	*/
	void Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget);
private:
	RenderTarget m_monochromedTarget;		//モノクロ化させたい画面をコピーしてくるレンダーターゲット
	Sprite m_monochromedSprite;				//モノクロ化した画像
};

