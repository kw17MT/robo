#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::instance = nullptr;

void RenderingEngine::PrepareRendering()
{
	InitRenderTargets();
	InitSprites();
	InitLightCamera();
	m_postEffect.Init(m_captureDeffered, m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_velocityTarget);
}

void RenderingEngine::InitRenderTargets()
{
	m_mainRenderTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_captureDeffered.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_albedoTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_normalTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	m_specAndDepthTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
	m_velocityTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);
	m_forwardTarget.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_sky.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	m_shadow.InitShadowTarget();
};

void RenderingEngine::InitSprites()
{
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = 1280;
	m_mainSpriteData.m_height = 720;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSprite.Init(m_mainSpriteData);

	m_effectedDefferedData.m_textures[0] = &m_captureDeffered.GetRenderTargetTexture();
	m_effectedDefferedData.m_width = 1280;
	m_effectedDefferedData.m_height = 720;
	m_effectedDefferedData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_effectedDeffered.Init(m_effectedDefferedData);

	m_forwardData.m_textures[0] = &m_sky.GetRenderTargetTexture();
	m_forwardData.m_width = 1280;
	m_forwardData.m_height = 720;
	m_forwardData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_forwardData.m_alphaBlendMode = AlphaBlendMode_Add;
	m_forward.Init(m_forwardData);

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

void RenderingEngine::DrawForwardRendering(RenderContext& rc, RenderTarget& target)
{
	//�ŏI�o�͂̃��C�������_�[�^�[�Q�b�g�ɏ������݊J�n
	rc.WaitUntilToPossibleSetRenderTarget(target);
	rc.SetRenderTargetAndViewport(target);
	rc.ClearRenderTargetView(target);
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(target);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::DrawForwardRendering(RenderContext& rc)
{
	//�ŏI�o�͂̃��C�������_�[�^�[�Q�b�g�ɏ������݊J�n
	rc.WaitUntilToPossibleSetRenderTarget(m_forwardTarget);
	rc.SetRenderTargetAndViewport(m_forwardTarget);
	rc.ClearRenderTargetView(m_forwardTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_forwardTarget);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);
}

void RenderingEngine::DrawInDefferedRenderTarget(RenderContext& rc)
{
	//�ŏI�o�͂̃��C�������_�[�^�[�Q�b�g�ɏ������݊J�n
	rc.WaitUntilToPossibleSetRenderTarget(m_captureDeffered);
	rc.SetRenderTargetAndViewport(m_captureDeffered);
	rc.ClearRenderTargetView(m_captureDeffered);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	//m_forward.Draw(rc);

	//DrawForwardRendering(rc, m_captureDeffered);

	m_defferedLighting.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_captureDeffered);
}

void RenderingEngine::DrawInMainRenderTarget(RenderContext& rc)
{
	//�ŏI�o�͂̃��C�������_�[�^�[�Q�b�g�ɏ������݊J�n
	rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//rc.ClearRenderTargetView(m_mainRenderTarget);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	m_defferedLighting.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);
}

void RenderingEngine::Render(RenderContext& rc)
{
	m_mat.currentVPMatrix = g_camera3D->GetViewProjectionMatrix();

	//�e���쐬����
	m_shadow.Render(rc);

	rc.WaitUntilToPossibleSetRenderTarget(m_sky);
	rc.SetRenderTargetAndViewport(m_sky);
	rc.ClearRenderTargetView(m_sky);
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_sky);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);


	//m_shadow.RenderCascade(rc);

	//DrawForwardRendering(rc,m_mainRenderTarget);

	//�f�B�t�@�[�h���C�e�B���O���s���B
	m_defferedLighting.Render(rc);
	//���C���摜���쐬����B
	//DrawInMainRenderTarget(rc);

	DrawInDefferedRenderTarget(rc);
	//rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
	//rc.SetRenderTargetAndViewport(m_mainRenderTarget);
	//rc.ClearRenderTargetView(m_mainRenderTarget);
	////�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	//m_shadow.Draw(rc);
	//rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);


	rc.WaitUntilToPossibleSetRenderTarget(m_captureDeffered);
	rc.SetRenderTarget(
		m_captureDeffered.GetRTVCpuDescriptorHandle(),
		m_albedoTarget.GetDSVCpuDescriptorHandle()
	);
	
	SetRenderTypes(RenderingEngine::EnRenderTypes::forward);
	//�f�B�t�@�[�h���C�e�B���O���ꂽ���C���̉摜�������B
	GameObjectManager::GetInstance()->CallRenderWrapper(rc);
	//m_forward.Draw(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(m_captureDeffered);
	SetRenderTypes(RenderingEngine::EnRenderTypes::normal);

	//�|�X�g�G�t�F�N�g�����C���摜�Ɏ{���B
	m_postEffect.Render(rc, m_mainRenderTarget, m_captureDeffered);

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
	//m_forward.Draw(rc);
	m_mainSprite.Draw(rc);
	//m_effectedDeffered.Draw(rc);

	m_mat.prevVPMatrix = g_camera3D->GetViewProjectionMatrix(); 
	//m_prevViewProjMatrix = g_camera3D->GetViewProjectionMatrix();
}