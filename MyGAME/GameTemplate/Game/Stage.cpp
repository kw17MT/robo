#include "stdafx.h"
#include "Stage.h"

Vector3 Stage::GetPosition()
{
	Vector3 Pos = m_charaCon.GetPosition();
	return Pos;
}

bool Stage::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/stage_factory.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;


	model.Init(modeldata);

	//m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	return true;
}

void Stage::Update()
{
	//Vector3 moveSpeed;
	//m_charaCon.Execute(moveSpeed, 1.0f);

	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}
