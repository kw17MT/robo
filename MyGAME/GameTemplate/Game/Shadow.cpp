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
	////�V���h�E�̍쐬���s�����f���̃h���[
	////���C�g�J������RendeingEngine�Œ�`���ASkinModelRender���Ŏg�p���Ă���B
	//GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	//RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}