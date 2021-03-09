#include "stdafx.h"
#include "Guzai.h"
#include "ModelRender.h"
#include "math.h"
#include "Kitchen.h"


bool Guzai::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/aaa.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_modelUpAxis = enModelUpAxisY;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	Vector3 pos = { 0.0f,100.0f,-500.0f };

	m_charaCon.Init(0.0f, 0.0f, pos);

	return true;
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

void Guzai::Update()
{
	//スケルトンを更新。
	m_skeleton.Update(model.GetWorldMatrix());

	ModelRender* mr = FindGO<ModelRender>("player01");
	Vector3 plPos = mr->GetPosition();
	Vector3 GuzaiPos = m_charaCon.GetPosition();

	Vector3 Kitchen01 = { 900.0f, 0.0f, 0.0f };


	float guzai2Pl = (GuzaiPos.x - plPos.x) * (GuzaiPos.x - plPos.x) + (GuzaiPos.y - plPos.y) * (GuzaiPos.y - plPos.y) + (GuzaiPos.z - plPos.z) * (GuzaiPos.z - plPos.z);
	guzai2Pl = sqrt(guzai2Pl);

	//Aボタンを押したとき、プレイヤーは何も持っていない　100より近い位置にいる。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (mr->have == 0 && guzai2Pl < 200.0f) {
			state = 1;
			mr->have = 1;
		}
	}
	//持たれていたら具材の位置をプレイヤーの上にする。
	if (state == 1) {
		plPos.y += 100.0f;
		SetPosition(plPos);
	}

	float Diff2Kit = (Kitchen01.x - plPos.x) * (Kitchen01.x - plPos.x) + (Kitchen01.y - plPos.y) * (Kitchen01.y - plPos.y) + (Kitchen01.z - plPos.z) * (Kitchen01.z - plPos.z);
	Diff2Kit = sqrt(Diff2Kit);

	//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
	//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
	if (g_pad[0]->IsTrigger(enButtonB)) {
		if (state == 1 && Diff2Kit < 400.0f) {
			Kitchen* ki = FindGO<Kitchen>("kitchen");
			ki->PlusStack();

			mr->have = 0;
		
			DeleteGO(this);
		}
	}

	//持たれていない　且つ　一度も置かれていない
	if (state == 0 && put == 0) {
		Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
		time++;
		if (time < 500) {
			moveSpeed.z = 2.0f;
		}
		if (time >= 500 && time < 600) {
			moveSpeed.x = 2.0f;
		}
		if (time >= 600) {
			moveSpeed.z = -2.0f;
		}
		if (GuzaiPos.z < -1000.0f) {
			time = 0;
			DeleteGO(this);
		}
		m_charaCon.Execute(moveSpeed, 1.0f);
	}
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}