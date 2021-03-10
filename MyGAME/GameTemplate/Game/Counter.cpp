#include "stdafx.h"
#include "Counter.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Kitchen.h"

bool Counter::Start()
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

	m_charaCon.Init(0.0f, 0.0f, CounterPos);

	CounterPos2.y += 100.0f;

	return true;
}

bool Counter::Judge()
{
	Kitchen* ki = FindGO<Kitchen>("kitchen");
	ModelRender* pl = FindGO<ModelRender>("player01");

	for (int i = 0; i < ki->GetStackNum(); i++) {
		if (TomatoOnly[i] != pl->GuzaiNo[i]) {
			return false;
		}
		else {
			return true;
		}
	}
}
	

//Burgerは出現している。消えてない。
void Counter::Delete()
{
	ModelRender* pl = FindGO<ModelRender>("player01");
	Vector3 plPos = pl->GetPosition();

	float pl2Counter = (plPos.x - CounterPos.x) * (plPos.x - CounterPos.x) + (plPos.y - CounterPos.y) * (plPos.y - CounterPos.y) + (plPos.z - CounterPos.z) * (plPos.z - CounterPos.z);
	pl2Counter = sqrt(pl2Counter);

	//プレイヤーがバーガーをもっていたら
	if (pl->have == 2) {
		Burger* bu = FindGO<Burger>("burger");

		//キッチンに置く準備
		if (g_pad[0]->IsPress(enButtonB) && pl2Counter < 200.0f) {
			if (Judge() == true) {
				bu->putOnKitchen = 1;
			}
		}

		if (bu->putOnKitchen == 1) {
			Delay++;
			bu->SetPosition(CounterPos2);

			if (Delay == 30) {
				DeleteGO(bu);
				Delay = 0;
				pl->have = 0;
			}
			
		}
	}
}

//bool Counter::Judge()
//{
//	Burger* bu = FindGO<Burger>("burger");
//	ModelRender* pl = FindGO<ModelRender>("player01");
//
//	int num = 5;
//	for (int i = 0; i < num; i++) {
//		
//	}
//
//}

void Counter::Update()
{
	Delete();
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}