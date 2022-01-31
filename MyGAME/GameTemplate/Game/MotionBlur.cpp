#include "stdafx.h"
#include "MotionBlur.h"

void MotionBlur::InitSprite(RenderTarget& mainRenderTarget, RenderTarget& normalTarget, RenderTarget& velocityTarget)
{
	//ベロシティマップにガウシアンブラーを適用
	m_gaussian[0].Init(&velocityTarget.GetRenderTargetTexture());
	m_gaussian[1].Init(&m_gaussian[0].GetBokeTexture());
	m_gaussian[2].Init(&m_gaussian[1].GetBokeTexture());
	m_gaussian[3].Init(&m_gaussian[2].GetBokeTexture());

	//モーションブラーで使うレンダーターゲットの初期化
	m_motionBlurTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	SpriteInitData motionBlurData;
	motionBlurData.m_width = 1280;
	motionBlurData.m_height = 720;
	motionBlurData.m_textures[0] = &m_motionBlurTarget.GetRenderTargetTexture();
	motionBlurData.m_textures[1] = &velocityTarget.GetRenderTargetTexture();
	motionBlurData.m_fxFilePath = "Assets/shader/MotionBlur.fx";
	motionBlurData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	motionBlurData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	motionBlurData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	motionBlurData.m_colorBufferFormat[3] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_motionBlurSprite.Init(motionBlurData);
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
	rc.WaitUntilFinishDrawingToRenderTarget(m_motionBlurTarget);

	//メインレンダーターゲットの画面にガウシアンブラーを掛ける
	m_gaussian[0].ExecuteOnGPU(rc, 15);
	m_gaussian[1].ExecuteOnGPU(rc, 15);
	m_gaussian[2].ExecuteOnGPU(rc, 15);
	m_gaussian[3].ExecuteOnGPU(rc, 15);

	//モーションブラー画像をメインレンダーターゲットに加算
	rc.WaitUntilToPossibleSetRenderTarget(targetToApply);
	rc.SetRenderTargetAndViewport(targetToApply);
	m_motionBlurSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(targetToApply);
}