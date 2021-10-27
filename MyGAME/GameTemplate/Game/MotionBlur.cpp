#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& defferedTarget, RenderTarget& normalTarget, RenderTarget& specAndDepthTarget, RenderTarget& velocityTarget)
{
	m_motionBlurTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	m_motionBlurData.m_width = 1280;
	m_motionBlurData.m_height = 720;
	m_motionBlurData.m_textures[0] = &m_motionBlurTarget.GetRenderTargetTexture();
	m_motionBlurData.m_textures[1] = &normalTarget.GetRenderTargetTexture();
	m_motionBlurData.m_textures[2] = &specAndDepthTarget.GetRenderTargetTexture();
	m_motionBlurData.m_textures[3] = &velocityTarget.GetRenderTargetTexture();
	m_motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	//m_motionBlurData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_motionBlurData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_motionBlurData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurSprite.Init(m_motionBlurData);
}

void MotionBlur::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& targetToApply)
{
	//モーションブラー適用後のレンダーターゲットを作成。
	rc.WaitUntilToPossibleSetRenderTarget(m_motionBlurTarget);
	rc.SetRenderTargetAndViewport(m_motionBlurTarget);
	rc.ClearRenderTargetView(m_motionBlurTarget);
	// メインレンダリングターゲットのテクスチャを使って
	// 2Dを全画面に書くだけに変更する。
	mainSprite.Draw(rc);
	//RenderingEngine::GetInstance()->GetDefferedSprite(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_motionBlurTarget);

	//モーションブラー画像をメインレンダーターゲットに加算
	rc.WaitUntilToPossibleSetRenderTarget(targetToApply);
	rc.SetRenderTargetAndViewport(targetToApply);
	m_motionBlurSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(targetToApply);
}