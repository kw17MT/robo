#include "stdafx.h"
#include "Unity.h"

Unity::Unity()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &skeleton;

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	model.Init(modeldata);
}

Unity::~Unity()
{

}

bool Unity::Start()
{
	return true;
}

void Unity::Update()
{

}

void Unity::Render(RenderContext& rc)
{
	model.Draw(rc);
}