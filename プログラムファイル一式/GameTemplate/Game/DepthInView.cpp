#include "stdafx.h"
#include "DepthInView.h"

void DepthInView::Init(RenderTarget& mainTarget, RenderTarget& depthTarget)
{
	//ブラーを掛けたいメインターゲットの絵で初期化
	m_depthGaussian.Init(&mainTarget.GetRenderTargetTexture());

	//被写界深度を適用した最終の画像
	SpriteInitData finalSpriteData;
	finalSpriteData.m_textures[0] = &m_depthGaussian.GetBokeTexture();
	finalSpriteData.m_textures[1] = &depthTarget.GetRenderTargetTexture();
	finalSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_colorBufferFormat[1] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;
	//シェーダー側で、深度値からワールド座標を計算したいため、カメラからみたプロジェクションビュー逆行列を送信
	finalSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData().ViewProjInverseMatrix;
	finalSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData().ViewProjInverseMatrix);
	finalSpriteData.m_fxFilePath = "Assets/shader/blur/depthInView.fx";
	finalSpriteData.m_alphaBlendMode = AlphaBlendMode_Trans;
	m_finalSprite.Init(finalSpriteData);
}

void DepthInView::Render(RenderContext& rc, RenderTarget& TargetToApply)
{
	//ガウシアンブラーを実行
	m_depthGaussian.ExecuteOnGPU(rc, 20);

	//書きたいレンダリングターゲットに設定してドロー
	rc.WaitUntilToPossibleSetRenderTarget(TargetToApply);
	rc.SetRenderTargetAndViewport(TargetToApply);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(TargetToApply);
}