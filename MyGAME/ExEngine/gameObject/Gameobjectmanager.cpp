/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"


GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
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
	//死亡フラグがついているゲームオブジェクトを破棄する。
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
	//レンダラーを変更するならここを改造していくと良い。
	//①メインレンダリングターゲットに切り替える
	//　レンダリングターゲットはメンバ変数にある
	//　コンストラクタで初期化。				→フォーマットの違いでERRORがでるかもしれない。それぞれのクラスで同じフォーマットで初期化する。

	/*UIやポストエフェクトの掛けたくない画像を最前面にドロー*************************************/
	/*rc.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	rc.SetRenderTargetAndViewport(mainRenderTarget);
	m_renderTypes = enRenderUI;
	CallRenderWrapper(rc);
	rc.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);*/
	/********************************************************************************************/

	/*現在のレンダーターゲットをフレームバッファにコピー*****************************************/
	//rc.SetRenderTarget(
	//	g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
	//	g_graphicsEngine->GetCurrentFrameBuffuerDSV()
	//);
	////描画するサイズを設定
	//rc.SetViewport(g_graphicsEngine->GetFrameBufferViewport());
	//rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	/********************************************************************************************/

	//最終の画面を表示
	//finalSprite.Draw(rc);

}