#include "stdafx.h"
#include "Guzai.h"
//#include "ModelRender.h"
#include "math.h"
#include "Kitchen.h"
#include "GuzaiGene.h"
#include "SpriteRender.h"
#include <ctime>
#include <cstdlib>

#include "PathMove.h"
#include<random>

#include "Player.h"
#include "SkinModelRender.h"

namespace
{
	const float MOVESPEED = 130.0f;
}


Guzai::Guzai()
{

}

//デストラクタ
Guzai::~Guzai()
{
	/*DeleteGO(pl01);
	DeleteGO(pl02);
	DeleteGO(ki01);
	DeleteGO(ki02);*/

	DeleteGO(m_skinModelRender);
}

Vector3 Guzai::GetPosition()
{
	/*Vector3 Pos = m_charaCon.GetPosition();
	return Pos;*/
	return m_position;
}

void Guzai::SetPosition(Vector3 pos)
{
	//m_charaCon.SetPosition(pos);
	m_position = pos;
}


void Guzai::ChangeGuzai(int num)
{
	TypeNo = num;

	switch (TypeNo) {
	case 0:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/cheese.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/cheese.tkm");
		NowModelPath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/egg.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/egg.tkm");
		NowModelPath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/lettuce.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/lettuce.tkm");
		NowModelPath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/patty.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/patty.tkm");
		NowModelPath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/tomato.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/tomato.tkm");
		NowModelPath = "Assets/modelData/gu/tomato.tkm";
		break;
	}
	
	m_skinModelRender->SetNewModel();
}

void Guzai::Move()
{
	

	//持たれていない　且つ　一度も置かれていない
	if (state == 0 && put == 0) {
		//移動させる。
		/*m_charaCon.*/SetPosition(m_pathMove.get()->Move());
		//最後のポイントまで到達したら。
		if (m_pathMove.get()->GetIsFinalPoint())
		{
			//削除する。
			DeleteGO(this);
			//DeleteGO(m_skinModelRender);
		}
	}
}

bool Guzai::Start()
{
	pl01 = FindGO<Player/*ModelRender*/>("player01");
	pl02 = FindGO<Player/*ModelRender*/>("player02");
	ki01 = FindGO<Kitchen>("kitchen01");
	ki02 = FindGO<Kitchen>("kitchen02");

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/cheese.tkm",nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	//m_skinModelRender->InitLight(g_lig);

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<int> rand(0, 4);

	TypeNo = rand(mt);

	////乱数でどの具材が流れてくるかを決める。
	//std::srand(time(NULL));
	//TypeNo = rand() % 5;

	switch (TypeNo) {
	case 0:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/cheese.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/cheese.tkm");
		NowModelPath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/egg.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/egg.tkm");
		NowModelPath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/lettuce.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/lettuce.tkm");
		NowModelPath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/patty.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/patty.tkm");
		NowModelPath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		//modeldata.m_tkmFilePath = "Assets/modelData/gu/tomato.tkm";
		m_skinModelRender->ChangeModel("Assets/modelData/gu/tomato.tkm");
		NowModelPath = "Assets/modelData/gu/tomato.tkm";
		break;
	}


	m_skinModelRender->SetNewModel();


	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_position/*m_charaCon.GetPosition()*/, MOVESPEED, enNormalLane, GuzaiNo);


	return true;
}

void Guzai::GrabNPut()
{
	if (GuzaiNo == 1) {
		Vector3 plPos = pl01->GetPosition();

		//Aボタンを押したとき、プレイヤーは何も持っていない　一定距離より近い位置にいる。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			if (pl01->have == 0 && guzai2Pl < 150.0f && targeted == true) {
				state = 1;
				pl01->have = 1;
			}
		}
		//持たれていたら具材の位置をプレイヤーの上にする。
		if (state == 1) {
			plPos.y += 100.0f;
			SetPosition(plPos);

			//ターゲット用のダミーを消す。
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			DeleteGO(targetDummy01);
		}

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (state == 1 && kit2Pl < 100.0f) {
				Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
				//キッチンに置いた具材の種類をプレイヤー側に保存
				pl01->GuzaiNo[ki01->GetStackNum()] = TypeNo;
				ki01->PlusStack();

				pl01->have = 0;
				
				//ターゲティングしていた具材を運んでいる最中は別の具材をターゲティングしたくないため、ここで初期化。
				targeted = false;
				pl01->SetTarget(targeted);
				isSetTargetDummy = false;
				decrementTime = holdTime;

				//具材の情報を消す。
				DeleteGO(this);
				//具材のモデルを消す。
				//DeleteGO(m_skinModelRender);
			}
		}
	}

	if (GuzaiNo == 2) {
		Vector3 plPos = pl02->GetPosition();

		//Aボタンを押したとき、プレイヤーは何も持っていない　100より近い位置にいる。
		if (g_pad[1]->IsTrigger(enButtonA)) {
			if (pl02->have == 0 && guzai2Pl < 150.0f && targeted == true) {
				state = 1;
				pl02->have = 1;
			}
		}
		//持たれていたら具材の位置をプレイヤーの上にする。
		if (state == 1) {
			plPos.y += 100.0f;
			SetPosition(plPos);

			//ターゲット用のダミーを消す。
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			DeleteGO(targetDummy02);
		}

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[1]->IsTrigger(enButtonB)) {
			if (state == 1 && kit2Pl < 100.0f) {
				Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
				//キッチンに置いた具材の種類をプレイヤー側に保存
				pl02->GuzaiNo[ki02->GetStackNum()] = TypeNo;
				ki02->PlusStack();

				pl02->have = 0;

				//ターゲティングしていた具材を運んでいる最中は別の具材をターゲティングしたくないため、ここで初期化。
				targeted = false;
				pl02->SetTarget(targeted);
				isSetTargetDummy = false;
				decrementTime = holdTime;

				//具材の情報を消す。
				DeleteGO(this);
				//具材のモデルを消す。
				//DeleteGO(m_skinModelRender);
			}
		}
	}
}

void Guzai::TargetingNPopDummy()
{
	//Vector3 GuzaiPos = m_charaCon.GetPosition();

	if (GuzaiNo == 1) {
		//具材との距離が一定以下　で　プレイヤーは何もロックしていなかったら。
			//近くの具材をターゲットし、プレイヤーのターゲット状態をTRUEに。
		if (guzai2Pl < TargetRangeNear && pl01->GetTargetState() == false) {
			targeted = true;
			pl01->SetTarget(targeted);
		}
		//ターゲットした具材がダミーを出していなかったら。
		//少し大きいダミーを具材と被るように出す。（色は後で真っ黒にするのでなんでもいい）
		if (targeted == 1 && isSetTargetDummy == false) {
			SkinModelRender* targetDummyOnGuzai01 = NewGO<SkinModelRender>(1, "targetdummy01");
			targetDummyOnGuzai01->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
			targetDummyOnGuzai01->InitShader("Assets/shader/model.fx", "VSMain", "FrontCulling", DXGI_FORMAT_R32G32B32A32_FLOAT);
			//targetdummyOnGuzai01->modeldata.m_psEntryPointFunc = "FrontCulling";
			targetDummyOnGuzai01->SetPosition(m_position/*GuzaiPos*/);
			targetDummyOnGuzai01->SetScale({ 1.3f,1.3f,1.3f });
			
			isSetTargetDummy = true;

		}

		//ここで一定以上離れてプレイヤーは何かしらターゲットしていたら（if文の後ろの条件は別の具材に影響を与えるのを防ぐため。
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl >= TargetRangeFar && pl01->GetTargetState() == true && targeted == true) {
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			if (targetDummy01 != nullptr) {
				decrementTime--;
				if (decrementTime == 0) {
					DeleteGO(targetDummy01);
					//DeleteGO(sp01);
					targeted = false;
					pl01->SetTarget(targeted);
					isSetTargetDummy = false;
					decrementTime = holdTime;
				}
			}
		}
	}
	if (GuzaiNo == 2) {
		//具材との距離が一定以下　で　プレイヤーは何もロックしていなかったら。
			//近くの具材をターゲットし、プレイヤーのターゲット状態をTRUEに。
		if (guzai2Pl < TargetRangeNear && pl02->GetTargetState() == false) {
			targeted = true;
			pl02->SetTarget(targeted);
		}
		//ターゲットした具材がダミーを出していなかったら。
		//少し大きいダミーを具材と被るように出す。（色は後で真っ黒にするのでなんでもいい）
		if (targeted == 1 && isSetTargetDummy == false) {
			SkinModelRender* targetDummyOnGuzai02 = NewGO<SkinModelRender>(1, "targetdummy02");
			targetDummyOnGuzai02->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
			targetDummyOnGuzai02->InitShader("Assets/shader/model.fx", "VSMain", "FrontCulling", DXGI_FORMAT_R32G32B32A32_FLOAT);
			//targetdummyOnGuzai02->modeldata.m_psEntryPointFunc = "FrontCulling";
			targetDummyOnGuzai02->SetPosition(m_position/*GuzaiPos*/);
			targetDummyOnGuzai02->SetScale({ 1.3f,1.3f,1.3f });
			isSetTargetDummy = true;
		}

		//ここで一定以上離れてプレイヤーは何かしらターゲットしていたら（if文の後ろの条件は別の具材に影響を与えるのを防ぐため。
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl >= TargetRangeFar && pl02->GetTargetState() == true && targeted == true) {
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {
				decrementTime--;
				if (decrementTime == 0) {
					DeleteGO(targetDummy02);
					targeted = false;
					pl02->SetTarget(targeted);
					isSetTargetDummy = false;
					decrementTime = holdTime;
				}
			}
		}
	}
}

void Guzai::Update()
{
	if (GuzaiNo == 1) {
		Vector3 plPos = pl01->GetPosition();
		//Vector3 GuzaiPos = m_charaCon.GetPosition();
		Vector3 kitchen01Pos = ki01->GetKitchenPos();

		//具材からプレイヤーまでの距離
		guzai2Pl = (m_position/*GuzaiPos*/.x - plPos.x) * (/*GuzaiPos*/m_position.x - plPos.x) + (/*GuzaiPos*/m_position.y - plPos.y) * (/*GuzaiPos*/m_position.y - plPos.y) + (/*GuzaiPos*/m_position.z - plPos.z) * (/*GuzaiPos*/m_position.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//キッチンからプレイヤーの距離
		kit2Pl = (kitchen01Pos.x - plPos.x) * (kitchen01Pos.x - plPos.x) + (kitchen01Pos.y - plPos.y) * (kitchen01Pos.y - plPos.y) + (kitchen01Pos.z - plPos.z) * (kitchen01Pos.z - plPos.z);
		kit2Pl = sqrt(kit2Pl);
		
		if (isSetFirstPos == false) {
			GuzaiGene* gene01 = FindGO<GuzaiGene>("gene01");
			SetPosition(gene01->GetPosition());

			isSetFirstPos = true;
		}

		TargetingNPopDummy();

		//PopTargetingIcon();

		GrabNPut();

		Move();
		
		//ダミーを動かすよう
		if (isSetTargetDummy == true && state != 1) {
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			if (targetDummy01 != nullptr) {
				targetDummy01->SetPosition(m_position/*GuzaiPos*/);
			}
		}
	}

	if (GuzaiNo == 2) {
		Vector3 plPos = pl02->GetPosition();
		//Vector3 GuzaiPos = m_charaCon.GetPosition();
		Vector3 kitchen02Pos = ki02->GetKitchenPos();

		//具材からプレイヤーへの距離
		guzai2Pl = (m_position/*GuzaiPos*/.x - plPos.x) * (/*GuzaiPos*/m_position.x - plPos.x) + (/*GuzaiPos*/m_position.y - plPos.y) * (/*GuzaiPos*/m_position.y - plPos.y) + (/*GuzaiPos*/m_position.z - plPos.z) * (/*GuzaiPos*/m_position.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//キッチンからプレイヤーへの距離
		kit2Pl = (kitchen02Pos.x - plPos.x) * (kitchen02Pos.x - plPos.x) + (kitchen02Pos.y - plPos.y) * (kitchen02Pos.y - plPos.y) + (kitchen02Pos.z - plPos.z) * (kitchen02Pos.z - plPos.z);
		kit2Pl = sqrt(kit2Pl);

		if (isSetFirstPos == false) {
			GuzaiGene* gene02 = FindGO<GuzaiGene>("gene02");
			SetPosition(gene02->GetPosition());
			isSetFirstPos = true;
		}

		TargetingNPopDummy();
		
		GrabNPut();

		Move();

		//ダミーを動かすよう
		if (isSetTargetDummy == true && state != 1) {
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {
				targetDummy02->SetPosition(m_position/*GuzaiPos*/);
			}
		}
	}
	//model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, GuzaiScale);
	m_skinModelRender->SetPosition(m_position);
}