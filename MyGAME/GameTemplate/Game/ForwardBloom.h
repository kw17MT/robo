#pragma once
class ForwardBloom
{
public:
	/**
	 * @brief ブルーム作成の初期化を行う
	 * @param rt 輝度抽出したいレンダリングターゲット
	*/
	void Init(RenderTarget& rt);

	/**
	 * @brief 川瀬式ブルームを作成しターゲットに描画
	 * @param rc レンダーコンテキスト
	 * @param TargetToApply ブルームを適用したいターゲット
	*/
	void Render(RenderContext& rc, RenderTarget& TargetToApply);

private:
	RenderTarget m_luminanceTarget;		//輝度取得用ターゲット
	GaussianBlur m_gaussianBlur[4];		//川瀬式ガウシアンブラー作成用メンバ
	Sprite m_bloomSprite;				//ブルーム画像
	Sprite m_finalSprite;				//ブルームを適用した画像
};

