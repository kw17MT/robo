#pragma once
class DefferedLighting
{
public:
	/**
	 * @brief ディファードライティング適用の画像初期化関数
	 * @param albedoMap アルベドマップ
	 * @param normalMap 法線マップ
	 * @param specAndDepthMap 鏡面反射、深度値マップ
	 * @param shadowMap シャドウマップ
	 * @param speedMap スピードマップ
	*/
	void InitSprite(RenderTarget& albedoMap, 
		RenderTarget& normalMap, 
		RenderTarget& specAndDepthMap, 
		RenderTarget& shadowMap,
		RenderTarget& velocityMap);

	/**
	 * @brief ディファードレンダリングを開始する
	 * @param rc レンダーコンテキスト
	*/
	void Render(RenderContext& rc);

	/**
	 * @brief 作成したディファードライティング済み画像を描画する。
	 * @param rc レンダーコンテキスト
	*/
	void Draw(RenderContext& rc);
private:									
	RenderTarget* m_defferedTargets[4];		//MRT用	
	SpriteInitData m_defferedSpriteData;	//ディファードレンダリング画像のパラメータ
	Sprite m_defferedSprite;				//最終の画像

	Texture m_groundTextures[4];
};

