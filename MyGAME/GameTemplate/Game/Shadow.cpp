#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//今はデプスシャドウ。
	m_shadowTarget.Create(
			2048,
			2048,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			m_clearColor
		);
}

void Shadow::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowTarget);
	rc.SetRenderTargetAndViewport(m_shadowTarget);
	rc.ClearRenderTargetView(m_shadowTarget);
	//m_renderTypes = enRenderShade;									//影するよ
	//シャドウの作成を行うモデルのドロー
	//ライトカメラはRendeingEngineで定義し、SkinModelRender内で使用している。
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowTarget);
}