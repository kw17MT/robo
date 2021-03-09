#include "stdafx.h"
#include "Kitchen.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Guzai.h"


bool Kitchen::Start()
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

	m_charaCon.Init(0.0f, 0.0f, KitchenPos);

	return true;
}

void Kitchen::Stack(int num)
{
	if (nextStackNum < stack) {
		StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
		StackedGuzai[nextStackNum]->put = 1;
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
	if (nextStackNum == 5) {
		Delay--;
		if (Delay == 0) {
			Delete();
			bur = NewGO<Burger>(0,"burger");
			bur->burgerExist = 1;
			ModelRender* pl = FindGO<ModelRender>("player01");
			pl->have = 1;
			Delay = 60;
		}
	}
}

void Kitchen::Update()
{
	Stack(stack);

	if (g_pad[0]->IsPress(enButtonX)) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
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