#include "stdafx.h"
#include "FXAA.h"

void FXAA::Init(RenderTarget& defferedTarget)
{
	SpriteInitData fxaaSpriteData;

	m_fxaaTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	fxaaSpriteData.m_width = 1280;
	fxaaSpriteData.m_height = 720;
	fxaaSpriteData.m_textures[0] = &defferedTarget.GetRenderTargetTexture();
	fxaaSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	fxaaSpriteData.m_fxFilePath = "Assets/shader/fxaa.fx";
	fxaaSpriteData.m_vsEntryPointFunc = "VSMain";
	fxaaSpriteData.m_psEntryPoinFunc = "PSMain";

	fxaaSpriteData.m_expandConstantBuffer = (void*)&s_data;
	fxaaSpriteData.m_expandConstantBufferSize = sizeof(s_data) +
		(16 - (sizeof(s_data) % 16));

	fxaaSpriteData.m_alphaBlendMode = AlphaBlendMode_None;

	m_finalSprite.Init(fxaaSpriteData);
}

void FXAA::Render(RenderContext& rc, RenderTarget& mainTarget)
{
	//メインレンダリングターゲットに書き込む
	rc.WaitUntilToPossibleSetRenderTarget(mainTarget);
	rc.SetRenderTargetAndViewport(mainTarget);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainTarget);
}