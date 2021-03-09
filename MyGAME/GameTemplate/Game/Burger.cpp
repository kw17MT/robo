#include "stdafx.h"
#include "Burger.h"
#include "ModelRender.h"

bool Burger::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/box.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	m_charaCon.Init(0.0f, 0.0f, pos);

	return true;
}

void Burger::SetPosition(Vector3 pos)
{
	m_charaCon.SetPosition(pos);
}

void Burger::Delete()
{
	Burger* bur = FindGO<Burger>("burger");
	DeleteGO(bur);
}

void Burger::GrabBurger()
{
	ModelRender* pl = FindGO<ModelRender>("player01");
	Vector3 plPos = pl->GetPosition();


}

void Burger::Update()
{
	if (g_pad[0]->IsPress(enButtonX)) {
		DeleteTimer++;
		if (burgerExist == 1 && DeleteTimer == 60) {
			Delete();
			DeleteTimer = 0;
			burgerExist = 0;
		}
	}

	//プレイヤーが持てるようにする。



	m_charaCon.SetPosition(pos);
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}
