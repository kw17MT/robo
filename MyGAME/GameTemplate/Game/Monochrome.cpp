#include "stdafx.h"
#include "Monochrome.h"

void Monochrome::Init()
{
	m_monochromedTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	SpriteInitData data;
	data.m_textures[0] = &m_monochromedTarget.GetRenderTargetTexture();
	data.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	data.m_expandConstantBuffer = (void*)&RenderingEngine::GetInstance()->GetMonochromeRate();
	data.m_expandConstantBufferSize = sizeof(m_monochromeRate);
	data.m_fxFilePath = "Assets/shader/monochrome.fx";
	data.m_psEntryPoinFunc = "PSMonochrome";
	data.m_width = 1280;
	data.m_height = 720;
	data.m_alphaBlendMode = AlphaBlendMode_Trans;

	m_monochromedSprite.Init(data);
}

void Monochrome::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget)
{
	//モーションブラー画像をメインレンダーターゲットに加算
	rc.WaitUntilToPossibleSetRenderTarget(m_monochromedTarget);
	rc.SetRenderTargetAndViewport(m_monochromedTarget);
	rc.ClearRenderTargetView(m_monochromedTarget);
	mainSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_monochromedTarget);

	rc.WaitUntilToPossibleSetRenderTarget(mainTarget);
	rc.SetRenderTargetAndViewport(mainTarget);
	m_monochromedSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainTarget);
}