#include "stdafx.h"
#include "FXAA.h"

void FXAA::Init(RenderTarget& defferedTarget)
{
	SpriteInitData fxaaSpriteData;

	m_fxaaTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);

	fxaaSpriteData.m_width = 1280;
	fxaaSpriteData.m_height = 720;
	//fxaaSpriteData.m_textures[0] = &m_fxaaTarget.GetRenderTargetTexture();
	fxaaSpriteData.m_textures[0] = &defferedTarget.GetRenderTargetTexture();
	fxaaSpriteData.m_fxFilePath = "Assets/shader/fxaa.fx";
	fxaaSpriteData.m_vsEntryPointFunc = "VSMain";
	fxaaSpriteData.m_psEntryPoinFunc = "PSMain";

	fxaaSpriteData.m_expandConstantBuffer = (void*)&s_data;
	fxaaSpriteData.m_expandConstantBufferSize = sizeof(s_data) +
		(16 - (sizeof(s_data) % 16));

	fxaaSpriteData.m_alphaBlendMode = AlphaBlendMode_None;

	m_finalSprite.Init(fxaaSpriteData);
}

void FXAA::Render(RenderContext& rc, RenderTarget& defferedTarget)
{
	//FXAAを適用するモデルのドローをレンダリングターゲットに行う
	//rc.WaitUntilToPossibleSetRenderTarget(m_fxaaTarget);
	//rc.SetRenderTargetAndViewport(m_fxaaTarget);
	//rc.ClearRenderTargetView(m_fxaaTarget);
	//s_data.s_width = static_cast<float>(g_graphicsEngine->GetFrameBufferWidth());
	//s_data.s_height = static_cast<float>(g_graphicsEngine->GetFrameBufferHeight());
	//RenderingEngine::GetInstance()->GetDefferedSprite(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(m_fxaaTarget);

	//メインレンダリングターゲットに書き込む
	rc.WaitUntilToPossibleSetRenderTarget(defferedTarget);
	rc.SetRenderTargetAndViewport(defferedTarget);
	m_finalSprite.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(defferedTarget);
}