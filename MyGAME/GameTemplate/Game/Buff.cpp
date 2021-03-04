#include "stdafx.h"
#include "Buff.h"
#include "ModelRender.h"
#include "ObjectGene.h"

bool Buff::Start()
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
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

Buff::~Buff()
{
	
}

Vector3 Buff::GetPosition()
{
	Vector3 Pos = m_charaCon.GetPosition();
	return Pos;
}

void Buff::SetPosition(Vector3 pos)
{
	m_charaCon.SetPosition(pos);
}

void Buff::Update()
{
	//スケルトンを更新。
	m_skeleton.Update(model.GetWorldMatrix());
	Vector3 BuffPos = m_charaCon.GetPosition();
	ModelRender* mr = FindGO<ModelRender>("player01");
	Vector3 plPos = mr->GetPosition();

	wait--;
	if (wait <= 0) {
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
			if (BuffPos.z < -1000.0f) {
				time = 0;
				DeleteGO(this);
			}
			m_charaCon.Execute(moveSpeed, 1.0f);
			wait = 0;
		}
	}
	
	float Buff2Pl = (BuffPos.x - plPos.x) * (BuffPos.x - plPos.x) + (BuffPos.y - plPos.y) * (BuffPos.y - plPos.y) + (BuffPos.z - plPos.z) * (BuffPos.z - plPos.z);
	Buff2Pl = sqrt(Buff2Pl);

	//Aボタンを押したとき、プレイヤーは何も持っていない　100より近い位置にいる。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (Buff2Pl < 200.0f) {
			DeleteGO(this);
		}
	}


	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}