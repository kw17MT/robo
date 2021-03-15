#include "stdafx.h"
#include "Kitchen.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Guzai.h"
#include "Counter.h"

Kitchen::Kitchen()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/ches.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	Vector3 KitchenPos = { 0.0f,0.0f,0.0f };

	m_charaCon.Init(0.0f, 0.0f, KitchenPos);
}

//具材をセットポジする
//if2種類で分岐させる。
void Kitchen::Stack(int num)
{
	if (KitchenNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");

		if (nextStackNum < stack) {
			StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
			StackedGuzai[nextStackNum]->put = 1;
			StackedGuzai[nextStackNum]->ChangeGuzai(pl01->GuzaiNo[nextStackNum]);

			Vector3 GuzaiPos = m_charaCon.GetPosition();
			GuzaiPos.y += stack * 100.0f;
			StackedGuzai[nextStackNum]->SetPosition(GuzaiPos);

			nextStackNum++;
		}
	}

	if (KitchenNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");

		if (nextStackNum < stack) {
			StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
			StackedGuzai[nextStackNum]->put = 1;
			StackedGuzai[nextStackNum]->ChangeGuzai(pl02->GuzaiNo[nextStackNum]);

			Vector3 GuzaiPos = m_charaCon.GetPosition();
			GuzaiPos.y += stack * 100.0f;
			StackedGuzai[nextStackNum]->SetPosition(GuzaiPos);

			nextStackNum++;
		}
	}
}

//積んでいる具材を全部消去する。
//スタックに使った変数を全部初期化。
void Kitchen::Delete()
{
	if (KitchenNo == 1) {
		for (int i = 0;i < nextStackNum; i++) {
			DeleteGO(StackedGuzai[i]);
		}
		Counter* co01 = FindGO<Counter>("counter01");
		co01->SetStackNum(stack);

		stack = 0;
		nextStackNum = 0;
		DeleteTimer = 0;
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		pl01->have = 0;
		//pl01->SetGuzaiNo9();
	}
	if (KitchenNo == 2) {
		for (int i = 0;i < nextStackNum; i++) {
			DeleteGO(StackedGuzai[i]);
		}
		Counter* co02 = FindGO<Counter>("counter02");
		co02->SetStackNum(stack);

		stack = 0;
		nextStackNum = 0;
		DeleteTimer = 0;
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		pl02->have = 0;
		//pl02->SetGuzaiNo9();
	}
}

// Delayは必要
// ないとエラー
void Kitchen::BornBurger()
{
	if (KitchenNo == 1) {
		if (nextStackNum >= 1 && g_pad[0]->IsPress(enButtonY)) {
			Delay--;
			if (Delay == 0) {
				ModelRender* pl01 = FindGO<ModelRender>("player01");

				//ここで具材が持っている種類No.をプレイヤーが持っているNo.格納用配列にいれていく。
				for (int i = 0;i < nextStackNum; i++) {
					pl01->GuzaiNo[i] = StackedGuzai[i]->GetTypeNo();
				}
				Delete();
				pl01->have = 1;
				bur = NewGO<Burger>(0, "burger01");
				bur->SetBurgerNo(1);
				bur->burgerExist = 1;

				Delay = 60;
			}
		}
	}
	if (KitchenNo == 2) {
		if (nextStackNum >= 1 && g_pad[1]->IsPress(enButtonY)) {
			Delay--;
			if (Delay == 0) {
				ModelRender* pl02 = FindGO<ModelRender>("player02");

				//ここで具材の種類No.をプレイヤーが持っているNo.格納用配列にいれていく。
				for (int i = 0;i < nextStackNum; i++) {
					pl02->GuzaiNo[i] = StackedGuzai[i]->GetTypeNo();
				}
				Delete();
				pl02->have = 1;
				bur = NewGO<Burger>(0, "burger02");
				bur->SetBurgerNo(2);
				bur->burgerExist = 1;

				Delay = 60;
			}
		}
	}
}

void Kitchen::ClearNo()
{
	if (KitchenNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");

		for (int i = 0;i < nextStackNum; i++) {
			pl01->GuzaiNo[i] = 9;
		}
	}
	if (KitchenNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");

		for (int i = 0;i < nextStackNum; i++) {
			pl02->GuzaiNo[i] = 9;
		}
	}
}

void Kitchen::Update()
{
	Stack(stack);

	if (g_pad[0]->IsPress(enButtonX) && KitchenNo == 1) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
			ClearNo();
			Delete();
		}
	}
	if (g_pad[1]->IsPress(enButtonX) && KitchenNo == 2) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
			ClearNo();
			Delete();
		}
	}

	//キッチンに5個以上具材があると取れないようにする。
	if (nextStackNum >= 5 && KitchenNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		pl01->have = 1;
	}
	if (nextStackNum >= 5 && KitchenNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		pl02->have = 1;
	}

	BornBurger();
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}