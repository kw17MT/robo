#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& sceneMap, RenderTarget& normalMap, RenderTarget& specAndDepthMap, RenderTarget& velocityMap)
{
	m_motionBlurTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	m_motionBlurData.m_width = 1280;
	m_motionBlurData.m_height = 720;
	m_motionBlurData.m_textures[0] = &m_motionBlurTarget.GetRenderTargetTexture();
	m_motionBlurData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	m_motionBlurData.m_textures[3] = &velocityMap.GetRenderTargetTexture();
	m_motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	m_motionBlurData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_motionBlurData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurSprite.Init(m_motionBlurData);
}

void MotionBlur::Render(RenderContext& rc, RenderTarget& targetToApply)
{
	//モーションブラー適用後のレンダーターゲットを作成。
	rc.WaitUntilToPossibleSetRenderTarget(m_motionBlurTarget);
	rc.SetRenderTargetAndViewport(m_motionBlurTarget);
	rc.ClearRenderTargetView(m_motionBlurTarget);
	//ディファードライティングした画像を取得したい
	//GameObjectManager::GetInstance()->CallRenderWrapper(rc);

	RenderingEngine::GetInstance()->GetDefferedSprite(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_motionBlurTarget);

	//モーションブラー画像をメインレンダーターゲットに加算
	rc.WaitUntilToPossibleSetRenderTarget(targetToApply);
	rc.SetRenderTargetAndViewport(targetToApply);
	m_motionBlurSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(targetToApply);
}