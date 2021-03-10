#include "stdafx.h"
#include "Kitchen.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Guzai.h"


bool Kitchen::Start()
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

	m_charaCon.Init(0.0f, 0.0f, KitchenPos);

	return true;
}

void Kitchen::Stack(int num)
{
	ModelRender* pl = FindGO<ModelRender>("player01");

	if (nextStackNum < stack) {
		StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
		StackedGuzai[nextStackNum]->put = 1;
		StackedGuzai[nextStackNum]->ChangeGuzai(pl->GuzaiNo[nextStackNum]);
		
		nextStackNum++;
	}
}

void Kitchen::Delete()
{
	for (int i = 0;i < nextStackNum; i++) {
		DeleteGO(StackedGuzai[i]);
	}
	stack = 0;
	nextStackNum = 0;
	DeleteTimer = 0;
	ModelRender* pl = FindGO<ModelRender>("player01");
	pl->have = 0;
}

// Delayは必要
// ないとエラー
//5個積んだらバーガーに変換
void Kitchen::BornBurger()
{
	if (nextStackNum >= 1 && g_pad[0]->IsPress(enButtonY)) {
		Delay--;
		if (Delay == 0) {
			ModelRender* pl = FindGO<ModelRender>("player01");
			
			//ここで具材が持っている種類No.をプレイヤーが持っているNo.格納用配列にいれていく。
			for (int i = 0;i < nextStackNum; i++) {
				pl->GuzaiNo[i] = StackedGuzai[i]->GetTypeNo();
			}
			Delete();
			pl->have = 1;
			bur = NewGO<Burger>(0,"burger");
			bur->burgerExist = 1;
			
			Delay = 60;
		}
	}
}

void Kitchen::ClearNo()
{
	ModelRender* pl = FindGO<ModelRender>("player01");

	for (int i = 0;i < nextStackNum; i++) {
		pl->GuzaiNo[i] = 0;
	}
}

void Kitchen::Update()
{
	Stack(stack);

	if (g_pad[0]->IsPress(enButtonX)) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
			ClearNo();
			Delete();
		}
	}

	//具材をキッチンの上に載せるための座標設定
	for (int i = 0;i < nextStackNum;i++) {
		Vector3 GuzaiPos = KitchenPos;
		GuzaiPos.y = (i + 1) * 100.0f;
		StackedGuzai[i]->SetPosition(GuzaiPos);
	}

	//キッチンに5個以上具材があると取れないようにする。
	if (nextStackNum >= 5) {
		ModelRender* pl = FindGO<ModelRender>("player01");
		pl->have = 1;
	}

	BornBurger();
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}