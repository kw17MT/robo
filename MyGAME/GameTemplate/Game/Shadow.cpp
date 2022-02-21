#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//レンダーターゲットを初期化する。解像度は2048x2048
	m_shadow.Create(2048, 2048, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);
}

void Shadow::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadow);
	rc.SetRenderTargetAndViewport(m_shadow);
	rc.ClearRenderTargetView(m_shadow);
	//シャドウキャスターのみ描く。
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

	//シャドウの作成を行うモデルのドロー
	//ライトカメラはRendeingEngineで定義し、LightManagerで統括し、SkinModelRender内で使用している。
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadow);
	//全てのオブジェクトを描くモードに戻す
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}