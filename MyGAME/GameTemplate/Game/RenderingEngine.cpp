#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::instance = nullptr;

void RenderingEngine::PrepareRendering()
{
	InitRenderTargets();
	InitSprites();
	InitLightCamera();
	m_postEffect.Init(m_mainRenderTarget, m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_velocityTarget);
}

void RenderingEngine::InitRenderTargets()
{
	m_mainRenderTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_albedoTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_normalTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	m_specAndDepthTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_velocityTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_shadow.InitShadowTarget();
};

void RenderingEngine::InitSprites()
{
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = 1280;
	m_mainSpriteData.m_height = 720;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_mainSprite.Init(m_mainSpriteData);

	m_defferedLighting.InitSprite(m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_shadow.GetShadowMap(), m_velocityTarget);
	m_shadow.InitCascade(m_mainRenderTarget, m_normalTarget, m_specAndDepthTarget);
}

void RenderingEngine::InitLightCamera()
{
	m_lightCamera.SetPosition(0.0f, 10000.0f, 0.0f);
	m_lightCamera.SetFar(100000.0f);
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	m_lightCamera.SetUp({ 1, 0, 0 });							//�J�����̏��X���W�ɂ��Ă���
	m_lightCamera.SetViewAngle(Math::DegToRad(120.0f));
	m_lightCamera.Update();
}

void RenderingEngine::DrawInMainRenderTarget(RenderContext& rc)
{
	//�ŏI�o�͂̃��C�������_�[�^�[�Q�b�g�ɏ������݊J�n
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	rc.ClearRenderTargetView(m_mainRenderTarget);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	m_defferedLighting.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::StartForwardRendering(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTarget(
		m_mainRenderTarget.GetRTVCpuDescriptorHandle(),
		m_albedoTarget.GetDSVCpuDescriptorHandle()
	);
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::DrawUI(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::ui);
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::Render(RenderContext& rc)
{
	//�J�����̃v���W�F�N�V�����r���[�s����擾
	m_mat.currentVPMatrix = g_camera3D->GetViewProjectionMatrix();

	//�e���쐬����
	m_shadow.Render(rc);

	//m_shadow.RenderCascade(rc);

	//�f�B�t�@�[�h���C�e�B���O���s���B
	m_defferedLighting.Render(rc);

	//���C���̍ŏI�\���ƂȂ�摜�����_�����O�^�[�Q�b�g�Ƀf�B�t�@�[�h���C�e�B���O�摜��`��
	DrawInMainRenderTarget(rc);

	//�f�B�t�@�[�h���C�e�B���O�摜�ɋ�A���z���t�H���[�h�I�ɕ`��
	StartForwardRendering(rc);

	//�|�X�g�G�t�F�N�g�����C���摜�Ɏ{���B
	m_postEffect.Render(rc, m_mainSprite, m_mainRenderTarget);

	//�Ō��UI��`�悷��
	DrawUI(rc);

	/*���݂̃����_�[�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[*****************************************/
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//�`�悷��T�C�Y��ݒ�
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//�o�����摜�̕\��
	m_mainSprite.Draw(rc);

	//�J������1�t���[���O�̃v���W�F�N�V�����r���[�s����擾
	m_mat.prevVPMatrix = g_camera3D->GetViewProjectionMatrix(); 
}