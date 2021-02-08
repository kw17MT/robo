#include "stdafx.h"
#include "Unity.h"

Unity::Unity()
{
	
}

Unity::~Unity()
{

}

bool Unity::Start()
{
	data.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	data.m_fxFilePath = "Assets/shader/model.fx";

	data.m_vsEntryPointFunc = "VSMain";
	data.m_vsSkinEntryPointFunc = "VSSkinMain";

	data.m_modelUpAxis = enModelUpAxisY;

	model.Init(data);

	return true;
}

void Unity::Update()
{
	Vector3 pos = { 0.0f,0.0f,0.0f };
	model.UpdateWorldMatrix(pos, g_quatIdentity, g_vec3One);
}

void Unity::Render(RenderContext& rc)
{
	model.Draw(rc);
}