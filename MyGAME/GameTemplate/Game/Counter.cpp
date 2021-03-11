#include "stdafx.h"
#include "Counter.h"
#include "ModelRender.h"
#include "Burger.h"
#include "Kitchen.h"
#include "Score.h"

Counter::Counter()
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

	m_charaCon.Init(0.0f, 0.0f, g_vec3One);
}

bool Counter::Start()
{
	return true;
}

bool Counter::Judge()
{
	if (CounterNo == 1) {
		Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
		ModelRender* pl01 = FindGO<ModelRender>("player01");

		for (int i = 0; i <= ki01->GetStackNum(); i++) {
			if (burger01[i] == pl01->GuzaiNo[i]) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	if (CounterNo == 2) {
		Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
		ModelRender* pl02 = FindGO<ModelRender>("player02");

		for (int i = 0; i <= ki02->GetStackNum(); i++) {
			if (burger02[i] == pl02->GuzaiNo[i]) {
				return true;
			}
			else {
				return false;
				break;
			}
		}
	}
}

//バーガーを最終的に消してスコアを発生させる。
//カウンターに近いところでBボタンを押すといったん載せて消す。
void Counter::Delete()
{
	if (CounterNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		Vector3 plPos = pl01->GetPosition();

		Vector3 CounterPos01 = m_charaCon.GetPosition();

		float pl2Counter = (plPos.x - CounterPos01.x) * (plPos.x - CounterPos01.x) + (plPos.y - CounterPos01.y) * (plPos.y - CounterPos01.y) + (plPos.z - CounterPos01.z) * (plPos.z - CounterPos01.z);
		pl2Counter = sqrt(pl2Counter);

		//プレイヤーがバーガーをもっていたら
		if (pl01->have == 2) {
			Burger* bu01 = FindGO<Burger>("burger01");

			//キッチンに置く準備
			//Judge関数でできたハンバーガーの組成があっていたらカウンターに置ける。
			if (g_pad[0]->IsPress(enButtonB) && pl2Counter < 200.0f) {
				if (Judge() == true) {
					bu01->putOnKitchen = 1;
				}
			}

			//置いたら30フレーム後に消去
			//ここでスコアアップさせたい。
			if (bu01->putOnKitchen == 1) {
				Delay++;
				CounterPos01.y += 100.0f;
				bu01->SetPosition(CounterPos01);

				if (Delay == 30) {
					//ここで積み上げてた具材の数をScoreに渡してあげる。
					Score* sco = FindGO<Score>("score");
					sco->SetBasePoint01(5);
					
					DeleteGO(bu01);
					Delay = 0;
					pl01->have = 0;
				}

			}
		}
	}

	if (CounterNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		Vector3 plPos = pl02->GetPosition();

		Vector3 CounterPos02 = m_charaCon.GetPosition();

		float pl2Counter = (plPos.x - CounterPos02.x) * (plPos.x - CounterPos02.x) + (plPos.y - CounterPos02.y) * (plPos.y - CounterPos02.y) + (plPos.z - CounterPos02.z) * (plPos.z - CounterPos02.z);
		pl2Counter = sqrt(pl2Counter);

		//プレイヤーがバーガーをもっていたら
		if (pl02->have == 2) {
			Burger* bu02 = FindGO<Burger>("burger02");

			//キッチンに置く準備
			//Judge関数でできたハンバーガーの組成があっていたらカウンターに置ける。
			if (g_pad[1]->IsPress(enButtonB) && pl2Counter < 200.0f) {
				if (Judge() == true) {
					bu02->putOnKitchen = 1;
				}
			}
		

			//置いたら30フレーム後に消去
			//ここでスコアアップさせたい。
			if (bu02->putOnKitchen == 1) {
				Delay++;
				CounterPos02.y += 100.0f;
				bu02->SetPosition(CounterPos02);

				if (Delay == 30) {
					//ここで積み上げてた具材の数をScoreに渡してあげる。
					Score* sco = FindGO<Score>("score");
					sco->SetBasePoint02(5);
					DeleteGO(bu02);
					Delay = 0;
					pl02->have = 0;
				}

			}
		}
	}
}

void Counter::Update()
{
	Delete();
	Judge();
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}