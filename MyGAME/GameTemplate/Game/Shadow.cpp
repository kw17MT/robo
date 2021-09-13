#include "stdafx.h"
#include "Shadow.h"

void Shadow::InitShadowTarget()
{
	//���̓f�v�X�V���h�E�B
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
	//m_renderTypes = enRenderShade;									//�e�����
	//�V���h�E�̍쐬���s�����f���̃h���[
	//���C�g�J������RendeingEngine�Œ�`���ASkinModelRender���Ŏg�p���Ă���B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowTarget);
}