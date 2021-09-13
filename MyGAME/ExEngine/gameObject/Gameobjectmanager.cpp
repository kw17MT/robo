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

	/*UI��|�X�g�G�t�F�N�g�̊|�������Ȃ��摜���őO�ʂɃh���[*************************************/
	/*rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	m_renderTypes = enRenderUI;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);*/
	/********************************************************************************************/

	/*���݂̃����_�[�^�[�Q�b�g���t���[���o�b�t�@�ɃR�s�[*****************************************/
	//rc.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);
	////�`�悷��T�C�Y��ݒ�
	//rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	//rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//�ŏI�̉�ʂ�\��
	//finalSprite.Draw(rc);

}