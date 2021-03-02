#include "stdafx.h"
#include "Guzai.h"


bool Guzai::Start()
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

	m_charaCon.Init(0.0f, 0.0f, g_vec3Zero);

	wait = exist * 50;

	return true;
}

Guzai::~Guzai()
{

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
	wait--;
	if (wait <= 0) {
		if (state == 0 && put == 0) {
			Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
			time++;
			if (time < 30) {
				moveSpeed.x = -1.0f;
			}
			if (time >= 30 && time < 210) {
				moveSpeed.z = 1.0f;
			}
			if (time >= 210 && time < 240) {
				moveSpeed.x = 1.0f;
			}
			if (time >= 240 && time < 420) {
				moveSpeed.z = -1.0f;
			}
			if (time == 420) {
				time = 0;
			}
			m_charaCon.Execute(moveSpeed, 1.0f);
			wait = 0;
		}
	}
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}

