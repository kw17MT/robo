#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";
	modeldata.m_vsEntryPointFunc = "VSMain";

	model.Init(modeldata);

	return true;
}

void BackGround::Update()
{

}

void BackGround::Render(RenderContext& rc)
{
	model.Draw(rc);
}