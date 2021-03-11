#include "stdafx.h"
#include "Burger.h"
#include "ModelRender.h"
#include "Kitchen.h"

Burger::Burger()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/box2.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	pos = { 0.0f,0.0f, 0.0f };

	m_charaCon.Init(0.0f, 0.0f, pos);
}

bool Burger::Start()
{
	if (BurgerNo == 1) {
		Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
		Vector3 KiPos01 = ki01->GetKitchenPos();
		KiPos01.y += 100.0f;
		SetPosition(KiPos01);
	}
	if (BurgerNo == 2) {
		Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
		Vector3 KiPos02 = ki02->GetKitchenPos();
		KiPos02.y += 100.0f;
		SetPosition(KiPos02);
	}

	return true;
}

void Burger::SetPosition(Vector3 pos)
{
	m_charaCon.SetPosition(pos);
}

void Burger::Delete()
{
	if (BurgerNo == 1) {
		Burger* bur01 = FindGO<Burger>("burger01");
		DeleteGO(bur01);
	}
	if (BurgerNo == 2) {
		Burger* bur02 = FindGO<Burger>("burger02");
		DeleteGO(bur02);
	}
}

//プレイヤーがバーガーを持つ。
void Burger::GrabBurger()
{

	if (BurgerNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		Vector3 plPos = pl01->GetPosition();
		Vector3 burPos = m_charaCon.GetPosition();

		float pl2Burger = (plPos.x - burPos.x) * (plPos.x - burPos.x) + (plPos.y - burPos.y) * (plPos.y - burPos.y) + (plPos.z - burPos.z) * (plPos.z - burPos.z);
		pl2Burger = sqrt(pl2Burger);

		if (g_pad[0]->IsPress(enButtonA) && pl2Burger < 200.0f) {
			pl01->have = 2;
		}

		if (pl01->have == 2) {
			pos = plPos;
			pos.y += 100.0f;
			if (putOnKitchen != 1) {
				m_charaCon.SetPosition(pos);
			}
		}
	}

	if (BurgerNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		Vector3 plPos = pl02->GetPosition();
		Vector3 burPos = m_charaCon.GetPosition();

		float pl2Burger = (plPos.x - burPos.x) * (plPos.x - burPos.x) + (plPos.y - burPos.y) * (plPos.y - burPos.y) + (plPos.z - burPos.z) * (plPos.z - burPos.z);
		pl2Burger = sqrt(pl2Burger);

		if (g_pad[1]->IsPress(enButtonA) && pl2Burger < 200.0f) {
			pl02->have = 2;
		}

		if (pl02->have == 2) {
			pos = plPos;
			pos.y += 100.0f;
			if (putOnKitchen != 1) {
				m_charaCon.SetPosition(pos);
			}
		}
	}
}

void Burger::ClearNo()
{
	if (BurgerNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");

		for (int i = 0;i < 10; i++) {
			pl01->GuzaiNo[i] = 0;
		}
	}
	if (BurgerNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");

		for (int i = 0;i < 10; i++) {
			pl02->GuzaiNo[i] = 0;
		}
	}
}

void Burger::Update()
{
	if (g_pad[0]->IsPress(enButtonX) && BurgerNo == 1) {
		DeleteTimer++;
		if (burgerExist == 1 && DeleteTimer == 60) {
			ClearNo();
			Delete();
			DeleteTimer = 0;
			burgerExist = 0;
		}
	}
	if (g_pad[1]->IsPress(enButtonX) && BurgerNo == 2) {
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
