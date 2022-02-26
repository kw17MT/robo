#include "stdafx.h"
#include "DefferedLighting.h"

void DefferedLighting::InitSprite(RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specAndDepthMap,
	RenderTarget& shadowMap,
	RenderTarget& velocityMap)
{
	//ディファードライティングに使用するテクスチャの初期化を行う
	m_defferedSpriteData.m_width = albedoMap.GetWidth();
	m_defferedSpriteData.m_height = albedoMap.GetHeight();
	m_defferedSpriteData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_textures[3] = &shadowMap.GetRenderTargetTexture();
	m_defferedSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_defferedSpriteData.m_colorBufferFormat[1] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_defferedSpriteData.m_colorBufferFormat[2] = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_defferedSpriteData.m_colorBufferFormat[3] = DXGI_FORMAT_R32_FLOAT;
	m_defferedSpriteData.m_fxFilePath = "Assets/shader/deffered/defferedSprite.fx";
	m_defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	m_defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	m_defferedSprite.Init(m_defferedSpriteData);

	//モデル描画時に以下のテクスチャに該当情報を書き込む
	m_defferedTargets[0] = &albedoMap;
	m_defferedTargets[1] = &normalMap;
	m_defferedTargets[2] = &specAndDepthMap;
	m_defferedTargets[3] = &velocityMap;
}

void DefferedLighting::Render(RenderContext& rc)
{
	//4つのテクスチャに絵を描く
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
	//全てのモデルを描画
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	//ゲームタイムを用いてエフェクトの再生進行度の更新
	EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
	//エフェクトの描画
	EffectEngine::GetInstance()->Draw();
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(m_defferedTargets), m_defferedTargets);
}

void DefferedLighting::Draw(RenderContext& rc)
{
	//ディファードライティング済みの絵の描画を行う
	m_defferedSprite.Draw(rc);
}