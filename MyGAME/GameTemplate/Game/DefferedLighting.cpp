#include "stdafx.h"
#include "DefferedLighting.h"

void DefferedLighting::InitTargets()
{
	m_albedoTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	m_normalTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	m_specAndDepthTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
}

void DefferedLighting::InitSprite(RenderTarget& shadowTarget)
{
	m_defferedSpriteData.m_width = 1280;
	m_defferedSpriteData.m_height = 720;
	m_defferedSpriteData.m_textures[0] = &m_albedoTarget.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[1] = &m_normalTarget.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[2] = &m_specAndDepthTarget.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[3] = &shadowTarget.GetRenderTargetTexture();
	m_defferedSpriteData.m_fxFilePath = "Assets/shader/deffered/defferedSprite.fx";
	m_defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	m_defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	m_defferedSprite.Init(m_defferedSpriteData);
}

void DefferedLighting::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	//m_renderTypes = enRenderNormal;
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);

	//m_defferedSprite.Draw(rc);
}

void DefferedLighting::Draw(RenderContext& rc)
{
	m_defferedSprite.Draw(rc);
}