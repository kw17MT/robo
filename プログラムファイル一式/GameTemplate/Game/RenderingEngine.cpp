#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine* RenderingEngine::instance = nullptr;

namespace
{
	const Vector3 INITIAL_LIGHTCAMERA_POS = { 0.0f, 10000.0f, 0.0f };	//���C�g�J�����̏����ʒu
	const int DEFAULT_WIDTH = 1280;										//�ʏ�̃e�N�X�`������
	const int DEFAULT_HEIGHT = 720;										//�ʏ�̃e�N�X�`���c��
	const float LIGHTCAMERA_FAR = 100000.0f;							//���C�g�J�����̃t�@�[�N���b�v
	const int LIGHTCAMERA_WIDTH = 500;									//���C�g�J�������f������
	const int LIGHTCAMERA_HEIGHT = 400;									//���C�g�J�������f���c��
}

void RenderingEngine::PrepareRendering()
{
	//�����_�[�^�[�Q�b�g�����ׂč쐬����
	InitRenderTargets();
	//�X�v���C�g�̏�����
	InitSprites();
	//���C�g�J�����̏�����
	InitLightCamera();
	//�|�X�g�G�t�F�N�g�Ŏg�������_�[�^�[�Q�b�g�̑��M
	m_postEffect.Init(m_mainRenderTarget, m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_velocityTarget);
}

void RenderingEngine::InitRenderTargets()
{
	//���C���Q�[���̉�ʂƂȂ�^�[�Q�b�g
	m_mainRenderTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//�A���x�h�}�b�v�������݂̃^�[�Q�b�g
	m_albedoTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//�@���}�b�v�������݂̃^�[�Q�b�g
	m_normalTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	//���ʔ��˗��Ɛ[�x�l�������݂̃^�[�Q�b�g
	m_specAndDepthTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//�x���V�e�B�}�b�v�������݂̃^�[�Q�b�g
	m_velocityTarget.Create(DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_D32_FLOAT);
	//�V���h�E�������݂̃^�[�Q�b�g�̏�����
	m_shadow.InitShadowTarget();
};

void RenderingEngine::InitSprites()
{
	//���C�������_�[�^�[�Q�b�g���g�����摜�̏�����
	m_mainSpriteData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
	m_mainSpriteData.m_width = DEFAULT_WIDTH;
	m_mainSpriteData.m_height = DEFAULT_HEIGHT;
	m_mainSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_mainSpriteData.m_colorBufferFormat[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_mainSprite.Init(m_mainSpriteData);

	//�f�B�t�@�[�h�����_�����O�ł��ꂼ��̃^�[�Q�b�g�Ƀ}�b�v����������
	m_defferedLighting.InitSprite(m_albedoTarget, m_normalTarget, m_specAndDepthTarget, m_shadow.GetShadowMap(), m_velocityTarget);
}

void RenderingEngine::InitLightCamera()
{
	//���C�g�J�����̍��W�ݒ�
	m_lightCamera.SetPosition(INITIAL_LIGHTCAMERA_POS);
	//���C�g�J�����̉f�����E
	m_lightCamera.SetFar(LIGHTCAMERA_FAR);
	//���C�g�J�����̃^�[�Q�b�g
	m_lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	//�J�����̏��X���W�ɂ��Ă���
	m_lightCamera.SetUp({ 1, 0, 0 });							
	//���C�g�J������180�x��]�����Ēn�ʂ�����悤�ɂ���
	m_lightCamera.SetViewAngle(Math::DegToRad(180.0f));
	//���s���e�ɂ���
	m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	//���C�g�J���������e�͈̔͐ݒ�
	m_lightCamera.SetWidth(LIGHTCAMERA_WIDTH);
	m_lightCamera.SetHeight(LIGHTCAMERA_HEIGHT);
	//�ݒ���X�V����
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

void RenderingEngine::DrawUI(RenderContext& rc)
{
	//���C���̃Q�[����ʂ�UI����������
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

	//�f�B�t�@�[�h���C�e�B���O���s���B
	m_defferedLighting.Render(rc);

	//���C���̍ŏI�\���ƂȂ�摜�����_�����O�^�[�Q�b�g�Ƀf�B�t�@�[�h���C�e�B���O�摜��`��
	DrawInMainRenderTarget(rc);

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