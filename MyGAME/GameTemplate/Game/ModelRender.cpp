#include "stdafx.h"
#include "ModelRender.h"


//インスタンス化するときにモデル、影のパスを入れておく。
void ModelRender::GiveData(const char* ModelPath, const char* ShaderPath)
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = ModelPath;
	modeldata.m_fxFilePath = ShaderPath;

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	

	model.Init(modeldata);
}