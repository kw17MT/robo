#include "stdafx.h"
#include "LensGhost.h"

void LensGhost::Init()
{
	m_ghostTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	m_gaussianBlur.Init(&m_ghostTarget.GetRenderTargetTexture());

	m_lensGhostData.m_width = 1280;
	m_lensGhostData.m_height = 720;
	m_lensGhostData.m_fxFilePath = "Assets/shader/LensGhost.fx";
	m_lensGhostData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();	//&m_ghostTarget.GetRenderTargetTexture(); 
	m_lensGhostData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_lensGhostData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_lensGhost.Init(m_lensGhostData);
}

void LensGhost::Render(RenderContext& rc, RenderTarget& target)
{
	//�����Y�S�[�X�g�������^�[�Q�b�g�ɐݒ�
	rc.WaitUntilToPossibleSetRenderTarget(m_ghostTarget);
	rc.SetRenderTargetAndViewport(m_ghostTarget);
	rc.ClearRenderTargetView(m_ghostTarget);
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::lensGhost);
	//���z��`��
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_ghostTarget);
	//�ʏ�`�惂�[�h�ɒ���
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	m_gaussianBlur.ExecuteOnGPU(rc, 100);

	//���C���ƂȂ�^�[�Q�b�g�Ƀ����Y�S�[�X�g���㏑������B
	rc.WaitUntilToPossibleSetRenderTarget(target);
	rc.SetRenderTargetAndViewport(target);
	m_lensGhost.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(target);
}