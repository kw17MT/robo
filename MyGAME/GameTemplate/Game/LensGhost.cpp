#include "stdafx.h"
#include "LensGhost.h"

void LensGhost::Init()
{
	//�S�[�X�g�����������_�����O�^�[�Q�b�g�̏�����
	m_ghostTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);

	//�������S�[�X�g�Ƀu���[���|���邽�߂̏�����
	m_gaussianBlur.Init(&m_ghostTarget.GetRenderTargetTexture());

	//�쐬����S�[�X�g�̉摜�̏�����
	SpriteInitData lensGhostData;
	lensGhostData.m_width = 1280;
	lensGhostData.m_height = 720;
	lensGhostData.m_fxFilePath = "Assets/shader/LensGhost.fx";
	lensGhostData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();
	lensGhostData.m_alphaBlendMode = AlphaBlendMode_Add;
	lensGhostData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_lensGhost.Init(lensGhostData);
}

void LensGhost::Render(RenderContext& rc, RenderTarget& target)
{
	//�����Y�S�[�X�g�������^�[�Q�b�g�ɐݒ�
	rc.WaitUntilToPossibleSetRenderTarget(m_ghostTarget);
	rc.SetRenderTargetAndViewport(m_ghostTarget);
	rc.ClearRenderTargetView(m_ghostTarget);
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::lensGhost);
	//���z������`��
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_ghostTarget);
	//�ʏ�`�惂�[�h�ɒ���
	RenderingEngine::GetInstance()->SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	//���z�������f���Ă���^�[�Q�b�g�Ƀu���[���|����
	m_gaussianBlur.ExecuteOnGPU(rc, 100);

	//���C���ƂȂ�^�[�Q�b�g�Ƀ����Y�S�[�X�g���㏑������B
	rc.WaitUntilToPossibleSetRenderTarget(target);
	rc.SetRenderTargetAndViewport(target);
	m_lensGhost.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(target);
}