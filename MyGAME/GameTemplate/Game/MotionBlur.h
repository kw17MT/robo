#pragma once
class MotionBlur
{
public:
	/**
	 * @brief モーションブラーを作成に使うスプライトの初期化
	 * @param defferedTarget 通常ゲーム画面
	 * @param normalTarget 法線マップ
	 * @param velocityTarget ベロシティマップ
	*/
	void InitSprite(RenderTarget& defferedTarget, RenderTarget& normalTarget, RenderTarget& velocityTarget);

	/**
	 * @brief モーションブラーを作成してメイン画面に加算合成する
	 * @param rc レンダーコンテキスト
	 * @param mainSprite メイン画面のスプライト
	 * @param targetToApply モーションブラーを適用するメイン画面
	*/
	void Render(RenderContext& rc, Sprite& mainSprite , RenderTarget& targetToApply);

private:
	RenderTarget m_motionBlurTarget;		//モーションブラーを作成するターゲット
	Sprite m_motionBlurSprite;				//作成したモーションブラー画像
	GaussianBlur m_gaussian[4];				//ガウシアンブラー
};

