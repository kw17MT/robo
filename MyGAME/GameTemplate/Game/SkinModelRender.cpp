#include "stdafx.h"
#include "SkinModelRender.h"

SkinModelRender::SkinModelRender()
{

}

SkinModelRender::~SkinModelRender()
{
	DeleteGO(this);
}

void SkinModelRender::Init(const char* modelFilePath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

	m_modelInitData.m_modelUpAxis = UpAxis;

	m_model.Init(m_modelInitData);

	//キャラコンの初期化
	m_charaCon.Init(40.0f, 100.0f, pos);
}

void SkinModelRender::Init(const char* modelFilePath, const char* skeletonPath, EnModelUpAxis UpAxis, Vector3 pos)
{
	m_modelInitData.m_tkmFilePath = modelFilePath;

	m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

	m_modelInitData.m_vsEntryPointFunc = "VSMain";
	m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

	m_modelInitData.m_modelUpAxis = UpAxis;

	m_modelInitData.m_expandConstantBuffer = &g_lig;
	m_modelInitData.m_expandConstantBufferSize = sizeof(g_lig);
	
	if (skeletonPath != nullptr) {
		m_skeleton.Init(skeletonPath);
		m_modelInitData.m_skeleton = &m_skeleton;
	}

	m_model.Init(m_modelInitData);
	//初期化終わり//

	//キャラコンの初期化
	m_charaCon.Init(40.0f, 100.0f, pos);
}

void SkinModelRender::ChangeModel(const char* newModelFilePath)
{
	m_modelInitData.m_tkmFilePath = newModelFilePath;
}


void SkinModelRender::InitShader(const char* shaderFilePath, const char* entryPointPath, const char* skinEntryPointPath, DXGI_FORMAT colorBuffer)
{
	m_modelInitData.m_fxFilePath = shaderFilePath;
	m_modelInitData.m_vsEntryPointFunc = entryPointPath;
	m_modelInitData.m_vsSkinEntryPointFunc = skinEntryPointPath;
	m_modelInitData.m_colorBufferFormat = colorBuffer;
}

void SkinModelRender::InitLight(Light light) {
	m_modelInitData.m_expandConstantBuffer = &light;
	m_modelInitData.m_expandConstantBufferSize = sizeof(light);
}

void SkinModelRender::InitBackGroundLight(Texture target, Matrix lightcamera, Matrix lightcamerasize)
{
	m_modelInitData.m_expandShaderResoruceView = &target;
	m_modelInitData.m_expandConstantBuffer = (void*)&lightcamera;
	m_modelInitData.m_expandConstantBufferSize = sizeof(lightcamerasize);
}

bool SkinModelRender::Start()
{
	return true;
}

void SkinModelRender::Update()
{
	//スケルトンを更新。
	
	m_skeleton.Update(m_model.GetWorldMatrix());

	m_model.UpdateWorldMatrix(m_position, m_rot, m_scale);
}