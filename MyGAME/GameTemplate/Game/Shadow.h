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
	RenderTarget& GetShadowMap()
	{
		return m_shadow;
	}
	
	//カスケードシャドウを作成する
	void RenderCascade(RenderContext& rc);

	struct Matrixes
	{
		Matrix s_LVPCMatrix[3];
		Matrix inv = g_camera3D->GetViewProjectionMatrix();
	};

	void InitCascade(RenderTarget& mainTarget, RenderTarget& normalTexture, RenderTarget& specAndDepthTexture);

	void Draw(RenderContext& rc);

private:
	float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };			//初期化する色
	RenderTarget m_shadow;										//影を書き込むターゲット

	/////////////////////////////////////////////////////////
	static const int m_cascadeShadowNum = 3;
	int m_nearArea = g_camera3D->GetNear();
	const float m_shadowArea[m_cascadeShadowNum] = { 1000, 5000, g_camera3D->GetFar() };
	RenderTarget m_cascadeShadowMap[m_cascadeShadowNum];

	SpriteInitData m_cascadeShadowData;
	Sprite m_cascadeSprite;

	Matrixes mat;
};

