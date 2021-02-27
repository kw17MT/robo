#include "stdafx.h"
#include "ModelRender.h"


/// <summary>
/// モデルを変更するときに使う
/// 変更したくない箇所はnullptr
/// </summary>
/// <param name="ModelPath">モデルのパス</param>
/// <param name="ShaderPath">シェーダーのパス</param>
void ModelRender::GiveData(const char* ModelPath, const char* ShaderPath)
{
	ModelInitData modeldata;

	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	if (ModelPath != nullptr) {
		modeldata.m_tkmFilePath = ModelPath;
	}
	
	modeldata.m_fxFilePath = "Assets/shader/model.fx";
	if (ShaderPath != nullptr) {
		modeldata.m_fxFilePath = ShaderPath;
	}

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	model.Init(modeldata);
}

bool ModelRender::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	return true;
}

void ModelRender::Update()
{
	//スケルトンを更新。
	m_skeleton.Update(model.GetWorldMatrix());

	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * -1.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * -1.0f;

	if (playerNo == 1) {
		moveSpeed.x = g_pad[0]->GetLStickXF() * -1.0f;
		moveSpeed.z = g_pad[0]->GetLStickYF() * -1.0f;
	}
	if (playerNo == 2) {
		moveSpeed.x = g_pad[0]->GetRStickXF() * -1.0f;
		moveSpeed.z = g_pad[0]->GetRStickYF() * -1.0f;
	}
	m_charaCon.Execute(moveSpeed, 1.0f);
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}

