#include "stdafx.h"
#include "Guzai.h"
#include "ModelRender.h"
#include "math.h"
#include "Kitchen.h"
#include <ctime>
#include <cstdlib>

Guzai::Guzai()
{
	//乱数でどの具材が流れてくるかを決める。
	std::srand(time(NULL));
	TypeNo = rand() % 5;

	switch (TypeNo) {
	case 0:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/tomato.tkm";
		break;
	}

	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	//m_skeleton.Init("Assets/modelData/unityChan.tks");
	//modeldata.m_skeleton = &m_skeleton;

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	modeldata.m_modelUpAxis = enModelUpAxisY;

	model.Init(modeldata);

	Vector3 pos = { 0.0f,0.0f,-500.0f };

	m_charaCon.Init(0.0f, 0.0f, pos);
}

Vector3 Guzai::GetPosition()
{
	Vector3 Pos = m_charaCon.GetPosition();
	return Pos;
}

void Guzai::SetPosition(Vector3 pos)
{
	m_charaCon.SetPosition(pos);
}


void Guzai::ChangeGuzai(int num)
{
	TypeNo = num;

	switch (TypeNo) {
	case 0:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		modeldata.m_tkmFilePath = "Assets/modelData/gu/tomato.tkm";
		break;
	}
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	model.Init(modeldata);
}

void Guzai::Update()
{
	//スケルトンを更新。
	//m_skeleton.Update(model.GetWorldMatrix());

	if (GuzaiNo == 1) {
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		Vector3 plPos = pl01->GetPosition();
		Vector3 GuzaiPos = m_charaCon.GetPosition();

		Vector3 Kitchen01 = { 900.0f, 0.0f, 0.0f };

		//具材からプレイヤーまでの距離
		float guzai2Pl = (GuzaiPos.x - plPos.x) * (GuzaiPos.x - plPos.x) + (GuzaiPos.y - plPos.y) * (GuzaiPos.y - plPos.y) + (GuzaiPos.z - plPos.z) * (GuzaiPos.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない　100より近い位置にいる。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			if (pl01->have == 0 && guzai2Pl < 150.0f) {
				state = 1;
				pl01->have = 1;
			}
		}
		//持たれていたら具材の位置をプレイヤーの上にする。
		if (state == 1) {
			plPos.y += 100.0f;
			SetPosition(plPos);
		}

		//キッチンからプレイヤーの距離
		float Diff2Kit = (Kitchen01.x - plPos.x) * (Kitchen01.x - plPos.x) + (Kitchen01.y - plPos.y) * (Kitchen01.y - plPos.y) + (Kitchen01.z - plPos.z) * (Kitchen01.z - plPos.z);
		Diff2Kit = sqrt(Diff2Kit);

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (state == 1 && Diff2Kit < 400.0f) {
				Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
				//キッチンに置いた具材の種類をプレイヤー側に保存
				pl01->GuzaiNo[ki01->GetStackNum()] = TypeNo;
				ki01->PlusStack();

				pl01->have = 0;

				DeleteGO(this);
			}
		}

		//持たれていない　且つ　一度も置かれていない
		if (state == 0 && put == 0) {
			Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
			timer++;
			if (timer < 500) {
				moveSpeed.z = 2.0f;
			}
			if (timer >= 500 && timer < 600) {
				moveSpeed.x = 2.0f;
			}
			if (timer >= 600) {
				moveSpeed.z = -2.0f;
			}
			if (GuzaiPos.z < -850.0f) {
				timer = 0;
				DeleteGO(this);
			}
			m_charaCon.Execute(moveSpeed, 1.0f);
		}
	}

	if (GuzaiNo == 2) {
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		Vector3 plPos = pl02->GetPosition();
		Vector3 GuzaiPos = m_charaCon.GetPosition();

		Vector3 Kitchen02 = { -900.0f, 0.0f, 0.0f };

		//具材からプレイヤーへの距離
		float guzai2Pl = (GuzaiPos.x - plPos.x) * (GuzaiPos.x - plPos.x) + (GuzaiPos.y - plPos.y) * (GuzaiPos.y - plPos.y) + (GuzaiPos.z - plPos.z) * (GuzaiPos.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない　100より近い位置にいる。
		if (g_pad[1]->IsTrigger(enButtonA)) {
			if (pl02->have == 0 && guzai2Pl < 150.0f) {
				state = 1;
				pl02->have = 1;
			}
		}
		//持たれていたら具材の位置をプレイヤーの上にする。
		if (state == 1) {
			plPos.y += 100.0f;
			SetPosition(plPos);
		}

		//キッチンからプレイヤーへの距離
		float Diff2Kit = (Kitchen02.x - plPos.x) * (Kitchen02.x - plPos.x) + (Kitchen02.y - plPos.y) * (Kitchen02.y - plPos.y) + (Kitchen02.z - plPos.z) * (Kitchen02.z - plPos.z);
		Diff2Kit = sqrt(Diff2Kit);

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[1]->IsTrigger(enButtonB)) {
			if (state == 1 && Diff2Kit < 400.0f) {
				Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
				//キッチンに置いた具材の種類をプレイヤー側に保存
				pl02->GuzaiNo[ki02->GetStackNum()] = TypeNo;
				ki02->PlusStack();

				pl02->have = 0;

				DeleteGO(this);
			}
		}

		//持たれていない　且つ　一度も置かれていない
		if (state == 0 && put == 0) {
			Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
			timer++;
			if (timer < 500) {
				moveSpeed.z = 2.0f;
			}
			if (timer >= 500 && timer < 600) {
				moveSpeed.x -= 2.0f;
			}
			if (timer >= 600) {
				moveSpeed.z = -2.0f;
			}
			if (GuzaiPos.z < -850.0f) {
				timer = 0;
				DeleteGO(this);
			}
			m_charaCon.Execute(moveSpeed, 1.0f);
		}
	}

	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}