/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;

	//���C���̃����_�[�^�[�Q�b�g�̏�����
	//�ŏI�̃����_�����O�^�[�Q�b�g�ɂ���
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	//�ŏI�\���p�̉摜�f�[�^
	finalSpriteData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	finalSpriteData.m_width = 1280;
	finalSpriteData.m_height = 720;
	finalSpriteData.m_fxFilePath = "Assets/shader/sprite.fx";

	finalSprite.Init(finalSpriteData);

	//�V���h�E�̃I�t�X�N���[�������_�����O�쐬
	shadowMap.Create(
		2048,
		2048,
		1,
		1,
		//DXGI_FORMAT_R8G8B8A8_UNORM,			//���e�V���h�E�Ŏg��
		DXGI_FORMAT_R32_FLOAT,					//�f�v�X�V���h�E�Ŏg��
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
	//���C�g�J�����̍쐬
	lightCamera.SetPosition(0.0f, 1000.0f, -500.0f);
	lightCamera.SetTarget(0.0f, 0.0f, 0.0f);
	lightCamera.SetUp({ 1, 0, 0});							//�J�����̏��X���W�ɂ��Ă���
	lightCamera.SetViewAngle(Math::DegToRad(145.0f));
	//���s���e�ɂ���ꍇ
	/*
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1280);
	lightCamera.SetHeight(720);
	*/
	lightCamera.Update();


	albedoMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	normalMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_UNKNOWN);
	specAndDepthMap.Create(1280, 720, 1, 1, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_UNKNOWN);

	defferedSpriteData.m_width = 1280;
	defferedSpriteData.m_height = 720;
	defferedSpriteData.m_textures[0] = &albedoMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[1] = &normalMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[2] = &specAndDepthMap.GetRenderTargetTexture();
	defferedSpriteData.m_textures[3] = &shadowMap.GetRenderTargetTexture();
	defferedSpriteData.m_fxFilePath = "Assets/shader/deffered/defferedSprite.fx";
	defferedSpriteData.m_alphaBlendMode = AlphaBlendMode_Add;
	defferedSpriteData.m_expandConstantBuffer = (void*)&LightManager::GetInstance().GetLightData();
	defferedSpriteData.m_expandConstantBufferSize = sizeof(LightManager::GetInstance().GetLightData());
	defferedSprite.Init(defferedSpriteData);

	forwardBloomTarget.Create(
		1280,   // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		720,  // �𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ���
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	m_forwardBloom.Init(forwardBloomTarget);
	m_postEffect.Init(mainRenderTarget);
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}


	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->UpdateWrapper();
		}
	}
}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	//�@���C�������_�����O�^�[�Q�b�g�ɐ؂�ւ���
	//�@�����_�����O�^�[�Q�b�g�̓����o�ϐ��ɂ���
	//�@�R���X�g���N�^�ŏ������B				���t�H�[�}�b�g�̈Ⴂ��ERROR���ł邩������Ȃ��B���ꂼ��̃N���X�œ����t�H�[�}�b�g�ŏ���������B
	
	/*�V���h�E�}�b�v�쐬************************************************************************/
	//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g�ɐݒ肷��B
	rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
	rc.SetRenderTargetAndViewport(shadowMap);
	rc.ClearRenderTargetView(shadowMap);
	m_renderTypes = enRenderShade;									//�e�����
	//�V���h�E�̍쐬���s�����f���̃h���[
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
	/********************************************************************************************/

	/*�f�B�t�@�[�h�쐬*****************************************************************************/
	rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.SetRenderTargetsAndViewport(ARRAYSIZE(defferedTargets), defferedTargets);
	rc.ClearRenderTargetViews(ARRAYSIZE(defferedTargets), defferedTargets);
	m_renderTypes = enRenderNormal;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(defferedTargets), defferedTargets);
	/********************************************************************************************/

	/*��������ŏI�I�ɕ\�������ʁi�摜�j���쐬*************************************************/
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	rc.ClearRenderTargetView(mainRenderTarget);
	//�f�B�t�@�[�h���C�e�B���O�������摜�̕`��
	defferedSprite.Draw(rc);
	//m_forwardBloom.Render(rc, mainRenderTarget);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	/********************************************************************************************/

	/*�t�H���[�h�����_�����O�ł̃u���[��*/
	rc.WaitUntilToPossibleSetRenderTarget(forwardBloomTarget);
	rc.SetRenderTargetAndViewport(forwardBloomTarget);
	rc.ClearRenderTargetView(forwardBloomTarget);
	m_renderTypes = enRenderLuminance;									//�u���[
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(forwardBloomTarget);
	m_forwardBloom.Render(rc, mainRenderTarget);
	/********************************************************************************************/
	
	/*�|�X�g�G�t�F�N�g���s��*********************************************************************/
	//�u���[����AA���s��
	m_postEffect.Render(rc, mainRenderTarget);
	/********************************************************************************************/

	/*UI��|�X�g�G�t�F�N�g�̊|�������Ȃ��摜���őO�ʂɃh���[*************************************/
	rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	m_renderTypes = enRenderUI;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
	/********************************************************************************************/

	/*���݂̃����_�[�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[*****************************************/
	rc.SetRenderTarget(
		g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
		g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	);
	//�`�悷��T�C�Y��ݒ�
	rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//�ŏI�̉�ʂ�\��
	finalSprite.Draw(rc);

}