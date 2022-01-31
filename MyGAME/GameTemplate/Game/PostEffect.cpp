#include "stdafx.h"
#include "PostEffect.h"
#include "GameDirector.h"

void PostEffect::Init(RenderTarget& mainRenderTarget,
	RenderTarget& albedoMap,
	RenderTarget& normalMap,
	RenderTarget& specDepthMap,
	RenderTarget& velocityMap)
{
	//被写界深度の初期化
	m_depthInView.Init(mainRenderTarget, specDepthMap);
	//ブルームの初期化
	m_bloom.Init(mainRenderTarget);
	//FXAAの初期化
	m_fxaa.Init(mainRenderTarget);
	//モーションブラーの初期化
	m_motionBlur.InitSprite(mainRenderTarget, normalMap, velocityMap);
	//レンズゴーストの初期化
	m_lensGhost.Init();
	//モノクロ化の初期化
	m_monochrome.Init();
}

void PostEffect::Render(RenderContext& rc, Sprite& mainSprite, RenderTarget& mainTarget)
{
	//ゲームシーンがタイトルならば
	if (GameDirector::GetInstance().GetGameScene() == enTitle)
	{
		//被写界深度適用
		m_depthInView.Render(rc, mainTarget);
	}
	//それ以外のシーンで
	else
	{
		//モーションブラー適用
		m_motionBlur.Render(rc, mainSprite, mainTarget);
	}
	//ブルーム適用
	m_bloom.Render(rc, mainTarget);
	//FXAA適用
	m_fxaa.Render(rc, mainTarget);
	//レンズゴースト適用
	m_lensGhost.Render(rc, mainTarget);
	//モノクロ化の数値があれば
	if (RenderingEngine::GetInstance()->GetMonochromeRate() != 0.0f)
	{
		//モノクロ化適用
		m_monochrome.Render(rc, mainSprite, mainTarget);
	}
}