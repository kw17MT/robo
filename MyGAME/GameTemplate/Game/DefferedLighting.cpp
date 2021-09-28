#include "stdafx.h"
#include "DefferedLighting.h"

void DefferedLighting::InitSprite(RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specAndDepthMap,
	RenderTarget& shadowMap,
	RenderTarget& velocityMap)
{
	m_defferedSpriteData.m_width = 1280;
	m_defferedSpriteData.m_height = 720;
	m_defferedSpriteData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[3] = &shadowMap.GetRenderTargetTexture();
	//m_defferedSpriteData.m_textures[4] = &velocityMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_fxFilePath = "Assets/shader/deffered/defferedSprite.fx";
	m_defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	m_defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	m_defferedSprite.Init(m_defferedSpriteData);

	m_defferedTargets[0] = &albedoMap;
	m_defferedTargets[1] = &normalMap;
	m_defferedTargets[2] = &specAndDepthMap;
	m_defferedTargets[3] = &velocityMap;
}

void DefferedLighting::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
}

void DefferedLighting::Draw(RenderContext& rc)
{
	m_defferedSprite.Draw(rc);
}