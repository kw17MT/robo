#include "stdafx.h"
#include "Ground.h"
#include "SkinModelRender.h"

bool Ground::Start()
{
	shadowMap.Create(
		1024,
		1024,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	lightCamera.SetPosition(0, 1000, 0);
	lightCamera.SetUp({ 1, 0, 0 });
	lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	lightCamera.Update();

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Floor.tkm",nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/shadowReciever.fx", nullptr, nullptr, DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_skinModelRender->InitBackGroundLight(shadowMap.GetRenderTargetTexture(), lightCamera.GetViewProjectionMatrix(), lightCamera.GetViewProjectionMatrix());

	

	/*modeldata.m_expandShaderResoruceView = &shadowMap.GetRenderTargetTexture();
	modeldata.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	modeldata.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());*/

	//modeldata.m_tkmFilePath = "Assets/modelData/Floor.tkm";
	//modeldata.m_fxFilePath = "Assets/shader/shadowReciever.fx";

	//modeldata.m_vsEntryPointFunc = "VSMain";
	//modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	//modeldata.m_modelUpAxis = enModelUpAxisZ;

	/*modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);*/
	//modeldata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	/*model.Init(modeldata);

	m_charaCon.Init(40.0f, 100.0f, m_position);*/

	return true;
}

void Ground::Update()
{
	m_skinModelRender->SetPosition(m_position);

	//model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}