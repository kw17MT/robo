#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{

}

void Shadow::Render(RenderContext& rc, RenderTarget& shadowMap)
{
	//RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);
	//rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	//rc.SetRenderTargetAndViewport(shadowMap);
	//rc.ClearRenderTargetView(shadowMap);
	//
	////シャドウの作成を行うモデルのドロー
	////ライトカメラはRendeingEngineで定義し、SkinModelRender内で使用している。
	//GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	//RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}