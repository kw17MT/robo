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

	pos = { 900.0f,100.0f, 0.0f };

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

//プレイヤーがバーガーを持つ。
void Burger::GrabBurger()
{
	ModelRender* pl = FindGO<ModelRender>("player01");
	Vector3 plPos = pl->GetPosition();

	float pl2Burger = (plPos.x - pos.x) * (plPos.x - pos.x) + (plPos.y - pos.y) * (plPos.y - pos.y) + (plPos.z - pos.z) * (plPos.z - pos.z);
	pl2Burger = sqrt(pl2Burger);

	if (g_pad[0]->IsPress(enButtonA) && pl2Burger < 200.0f) {
		pl->have = 2;
	}

	if (pl->have == 2) {
		pos = plPos;
		pos.y += 100.0f;
		if (putOnKitchen != 1) {
			m_charaCon.SetPosition(pos);
		}
	}
}

void Burger::ClearNo()
{
	ModelRender* pl = FindGO<ModelRender>("player01");

	for (int i = 0;i < 10; i++) {
		pl->GuzaiNo[i] = 0;
	}
}

void Burger::Update()
{
	if (g_pad[0]->IsPress(enButtonX)) {
		DeleteTimer++;
		if (burgerExist == 1 && DeleteTimer == 60) {
			ClearNo();
			Delete();
			DeleteTimer = 0;
			burgerExist = 0;
		}
	}

	GrabBurger();

	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}
