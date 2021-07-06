#include "stdafx.h"
#include "SkinModelRender.h"

SkinModelRender::~SkinModelRender()
{
	DeleteGO(this);
}

//モデルを通常描画するための初期化
void SkinModelRender::Init(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* pLig = nullptr)
{
	//モデルのファイルパス設定
	m_modelInitData.m_tkmFilePath = modelFilePath;
	//モデルが使用するシェーダー（下はPBRのみ）
	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//頂点シェーダー設定
	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSMain";
	//使う色の範囲設定
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//どの軸を上にするか
	m_modelInitData.m_modelUpAxis = UpAxis;
	//もし、任意のライトがあるならば
	if (pLig != nullptr) {
		m_modelInitData.m_expandConstantBuffer = &pLig;
		m_modelInitData.m_expandConstantBufferSize = sizeof(pLig);
	}
	//なかったら共通のやつを使う
	else {
		m_modelInitData.m_expandConstantBuffer = &g_lig;
		m_modelInitData.m_expandConstantBufferSize = sizeof(g_lig);
	}

	//モデルのスケルトンがあるなら
	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);

	//キャラコンの初期化
	//m_charaCon.Init(0.0f, 0.0f, pos);
}

//影を出すための初期化
void SkinModelRender::InitForCastShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_shadowData.m_tkmFilePath = modelFilePath;

	/****************************************************/
	/*	デプスシャドウを適用							*/
	/*	シャドウマップのカラーバッファーも変更すること　*/
	/****************************************************/
	m_shadowData.m_fxFilePath = "Assets/shader/shadow/drawDepthShadowMap.fx";
	m_shadowData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;


	/****************************************************/
	/*	投影シャドウを適用								*/
	/*	シャドウマップのカラーバッファーも変更すること　*/
	/****************************************************/
	//m_shadowData.m_fxFilePath = "Assets/shader/shadow/drawProjectionShadowMap.fx";
	//m_shadowData.m_colorBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

	m_shadowData.m_vsEntryPointFunc = "VSMain";
	m_shadowData.m_vsSkinEntryPointFunc = "VSMain";


	m_shadowData.m_modelUpAxis = UpAxis;	

	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_shadowData.m_skeleton = &m_skeleton;
	}

	m_shadow.Init(m_shadowData);
	//初期化終わり//

	//キャラコンの初期化
	//m_charaCon.Init(0.0f, 0.0f, pos);

	m_isCastShadow = true;
}

void SkinModelRender::InitForRecieveShadow(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos, Light* lig = nullptr)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	/****************************************************/
	/*	投影シャドウを適用								*/
	/****************************************************/
	//m_modelInitData.m_fxFilePath = "Assets/shader/shadow/projectionShadowReciever.fx";

	/****************************************************/
	/*	デプスシャドウを適用							*/
	/****************************************************/
	m_modelInitData.m_fxFilePath = "Assets/shader/shadow/depthShadowReciever.fx";

	//カラーバッファーのフォーマットは共通
	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_modelInitData.m_modelUpAxis = UpAxis;

	//シャドウマップのテクスチャ、ライトカメラのビュープロ行列の取得
	/*if (lig == nullptr) {
	m_modelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();
	m_modelInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_modelInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());
	}
	else {
		m_modelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();
		m_modelInitData.m_expandConstantBuffer = &lig;
		m_modelInitData.m_expandConstantBufferSize = sizeof(lig);
	}*/

	m_modelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();
	m_modelInitData.m_expandConstantBuffer = (void*)&a;
	m_modelInitData.m_expandConstantBufferSize = sizeof(a);
	
	/*PBRの追加を試みる******************/
	/*if (lig) {
		m_modelInitData.m_expandConstantBufferForLight = &lig;
		m_modelInitData.m_expandConstantBufferForLightSize = sizeof(lig);
	}*/
	/**********************************/

	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}


	m_model.Init(m_modelInitData);
	//初期化終わり//

	//キャラコンの初期化
	//m_charaCon.Init(0.0f, 0.0f, pos);
}

void SkinModelRender::InitAsFloor(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	m_modelInitData.m_fxFilePath = "Assets/shader/shadow/shadowRecieverForFloor.fx";

	m_modelInitData.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	m_modelInitData.m_modelUpAxis = UpAxis;

	m_modelInitData.m_expandShaderResoruceView = &m_shadowMap.GetRenderTargetTexture();
	m_modelInitData.m_expandConstantBuffer = (void*)&m_lightCamera.GetViewProjectionMatrix();
	m_modelInitData.m_expandConstantBufferSize = sizeof(m_lightCamera.GetViewProjectionMatrix());


	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);
}

void SkinModelRender::ChangeModel(const char* newModelFilePath)
{
	m_modelInitData.m_tkmFilePath = newModelFilePath;
	if (m_isCastShadow) {
		m_shadowData.m_tkmFilePath = newModelFilePath;
	}
}


void SkinModelRender::InitShader(const char* shaderFilePath, const char* entryPointPath, const char* skinEntryPointPath, DXGI_FORMAT colorBuffer)
{
	m_modelInitData.m_fxFilePath = shaderFilePath;
	m_modelInitData.m_vsEntryPointFunc = entryPointPath;
	m_modelInitData.m_vsSkinEntryPointFunc = skinEntryPointPath;
	m_modelInitData.m_colorBufferFormat = colorBuffer;
}

void SkinModelRender::InitLight(Light& light) {
	m_modelInitData.m_expandConstantBuffer = &light;
	m_modelInitData.m_expandConstantBufferSize = sizeof(light);
}

//void SkinModelRender::InitAnimation(AnimationClip* animationClip, int animationNum)
//{
//	m_animationClip = animationClip;
//
//	m_animation.Init(m_skeleton, m_animationClip, animationNum);
//}

//void SkinModelRender::PlayAnimation(int animNo, float interpolateTime)
//{
//	m_animation.Play(animNo, interpolateTime);
//}

void SkinModelRender::Update()
{
	//スケルトンを更新。
	m_skeleton.Update(m_model.GetWorldMatrix());

	//m_animation.Progress(GameTime().GetFrameDeltaTime());
	
	m_model.UpdateWorldMatrix(m_position, m_rot, m_scale);
	m_shadow.UpdateWorldMatrix(m_position, m_rot, m_scale);
}