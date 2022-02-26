#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//�����_�[�^�[�Q�b�g������������B�𑜓x��2048x2048
	m_shadow.Create(2048, 2048, 1, 1, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_D32_FLOAT, m_clearColor);
}

void Shadow::Render(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadow);
	rc.SetRenderTargetAndViewport(m_shadow);
	rc.ClearRenderTargetView(m_shadow);
	//�V���h�E�L���X�^�[�̂ݕ`���B
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::shadow);

	//�V���h�E�̍쐬���s�����f���̃h���[
	//���C�g�J������RendeingEngine�Œ�`���ALightManager�œ������ASkinModelRender���Ŏg�p���Ă���B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadow);
	//�S�ẴI�u�W�F�N�g��`�����[�h�ɖ߂�
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}