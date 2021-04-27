#include "stdafx.h"
#include "Buff.h"
//#include "ModelRender.h"
#include "GuzaiGene.h"

#include "PathMove.h"

#include "Player.h"

namespace
{
	//a
	const float MOVESPEED = 130.0f;
}

Buff::Buff() 
{
	ModelInitData modeldata;
	modeldata.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	modeldata.m_fxFilePath = "Assets/shader/model.fx";

	modeldata.m_vsEntryPointFunc = "VSMain";
	modeldata.m_vsSkinEntryPointFunc = "VSSkinMain";

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	modeldata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	modeldata.m_modelUpAxis = enModelUpAxisY;

	m_skeleton.Init("Assets/modelData/unityChan.tks");
	modeldata.m_skeleton = &m_skeleton;

	model.Init(modeldata);

	Vector3 pos = { 0.0f,100.0f,-500.0f };

	m_charaCon.Init(0.0f, 0.0f, pos);

	
}

bool Buff::Start()
{
	m_charaCon.SetPosition({ 0.0f, 0.0f, -1000.0f });

	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_charaCon.GetPosition(), MOVESPEED, enNormalLane, BuffNo);

	return true;
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

	//移動させる。
	m_charaCon.SetPosition(m_pathMove.get()->Move());
	//最後のポイントまで到達したら。
	if (m_pathMove.get()->GetIsFinalPoint())
	{
	

		GuzaiGene* gg = nullptr;
		if (BuffNo == 1)
		{
			gg = FindGO<GuzaiGene>("gene01");
		}
		else if (BuffNo == 2)
		{
			gg = FindGO<GuzaiGene>("gene02");
		}

		gg->Buffnum = 0;

		//削除する。
		DeleteGO(this);
	}

	

	if (BuffNo == 1) {
		Vector3 BuffPos = m_charaCon.GetPosition();
		/*ModelRender*/Player* pl01 = FindGO<Player/*ModelRender*/>("player01");
		Vector3 plPos01 = pl01->GetPosition();

		//バフアイテムとプレイヤーの距離を測る。
		float Buff2Pl = (BuffPos.x - plPos01.x) * (BuffPos.x - plPos01.x) + (BuffPos.y - plPos01.y) * (BuffPos.y - plPos01.y) + (BuffPos.z - plPos01.z) * (BuffPos.z - plPos01.z);
		Buff2Pl = sqrt(Buff2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない且つ100より近い位置にいる。
		if (g_pad[0]->IsTrigger(enButtonA) && pl01->have == 0) {
			if (Buff2Pl < 150.0f) {
				//プレイヤーにバフアイテムの効果をあたえる。
				//TRUEにすると移動速度がアップする。プレイヤー側で時間を測っていて、向こう側で効果を消す。
				//TODO バフをプレイヤーに付与。
				DeleteGO(this);

				pl01->SetEnItem(enBuffItem);
			}
		}
	}

	if (BuffNo == 2) {
		Vector3 BuffPos = m_charaCon.GetPosition();
		Player/*ModelRender*/* pl02 = FindGO<Player/*ModelRender*/>("player02");
		Vector3 plPos02 = pl02->GetPosition();

		//バフアイテムとプレイヤーの距離を測る。
		float Buff2Pl = (BuffPos.x - plPos02.x) * (BuffPos.x - plPos02.x) + (BuffPos.y - plPos02.y) * (BuffPos.y - plPos02.y) + (BuffPos.z - plPos02.z) * (BuffPos.z - plPos02.z);
		Buff2Pl = sqrt(Buff2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない且つ150より近い位置にいる。
		if (g_pad[1]->IsTrigger(enButtonA) && pl02->have == 0) {
			if (Buff2Pl < 150.0f) {
				//プレイヤーにバフアイテムの効果をあたえる。
				//TRUEにすると移動速度がアップする。プレイヤー側で時間を測っていて、向こう側で効果を消す。
				DeleteGO(this);

				pl02->SetEnItem(enBuffItem);
			}
		}
	}

	/*if (BuffNo == 1) {
		Vector3 BuffPos = m_charaCon.GetPosition();
		ModelRender* pl01 = FindGO<ModelRender>("player01");
		Vector3 plPos01 = pl01->GetPosition();

		//時間経過とともに具材の座標を移動させる。
		//TODO バフアイテムの移動。
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

			ObjectGene* gene01 = FindGO<ObjectGene>("gene01");
			gene01->Buffnum = 0;
		}
		m_charaCon.Execute(moveSpeed, 1.0f);

		//バフアイテムとプレイヤーの距離を測る。
		float Buff2Pl = (BuffPos.x - plPos01.x) * (BuffPos.x - plPos01.x) + (BuffPos.y - plPos01.y) * (BuffPos.y - plPos01.y) + (BuffPos.z - plPos01.z) * (BuffPos.z - plPos01.z);
		Buff2Pl = sqrt(Buff2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない且つ100より近い位置にいる。
		if (g_pad[0]->IsTrigger(enButtonA) && pl01->have == 0) {
			if (Buff2Pl < 150.0f) {
				//プレイヤーにバフアイテムの効果をあたえる。
				//TRUEにすると移動速度がアップする。プレイヤー側で時間を測っていて、向こう側で効果を消す。
				//TODO バフをプレイヤーに付与。
				pl01->SetBuffAffect(true);
				DeleteGO(this);

				//TODO バフアイテムを使ったらこれを0にする。
				ObjectGene* gene01 = FindGO<ObjectGene>("gene01");
				gene01->Buffnum = 0;
			}
		}
	}*/
	/*if (BuffNo == 2) {
		Vector3 BuffPos = m_charaCon.GetPosition();
		ModelRender* pl02 = FindGO<ModelRender>("player02");
		Vector3 plPos02 = pl02->GetPosition();

		//時間経過とともに具材の座標を移動させる。
		Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
		time++;
		if (time < 500) {
			moveSpeed.z = 2.0f;
		}
		if (time >= 500 && time < 600) {
			moveSpeed.x = -2.0f;
		}
		if (time >= 600) {
			moveSpeed.z = -2.0f;
		}
		if (BuffPos.z < -1000.0f) {
			time = 0;
			DeleteGO(this);

			ObjectGene* gene02 = FindGO<ObjectGene>("gene02");
			gene02->Buffnum = 0;
		}
		m_charaCon.Execute(moveSpeed, 1.0f);

		//バフアイテムとプレイヤーの距離を測る。
		float Buff2Pl = (BuffPos.x - plPos02.x) * (BuffPos.x - plPos02.x) + (BuffPos.y - plPos02.y) * (BuffPos.y - plPos02.y) + (BuffPos.z - plPos02.z) * (BuffPos.z - plPos02.z);
		Buff2Pl = sqrt(Buff2Pl);

		//Aボタンを押したとき、プレイヤーは何も持っていない且つ150より近い位置にいる。
		if (g_pad[1]->IsTrigger(enButtonA) && pl02->have == 0) {
			if (Buff2Pl < 150.0f) {
				//プレイヤーにバフアイテムの効果をあたえる。
				//TRUEにすると移動速度がアップする。プレイヤー側で時間を測っていて、向こう側で効果を消す。
				pl02->SetBuffAffect(true);
				DeleteGO(this);

				ObjectGene* gene02 = FindGO<ObjectGene>("gene02");
				gene02->Buffnum = 0;
			}
		}
	}*/
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);
}