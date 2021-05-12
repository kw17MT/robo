#include "stdafx.h"
#include "Guzai.h"
#include "math.h"
#include "Kitchen.h"
#include "GuzaiGene.h"
#include "GuzaiOkiba.h"
#include "SkinModelRender.h"
#include "Player.h"
#include "PlayerGene.h"
#include "TrashCan.h"
#include<random>

#include <ctime>
#include <cstdlib>
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
	DeleteGO(m_skinModelRender);
}

Vector3 Guzai::GetPosition()
{
	return m_position;
}

void Guzai::SetPosition(Vector3 pos)
{
	m_position = pos;
}

void Guzai::ChangeGuzai(int num)
{
	TypeNo = num;

	switch (TypeNo) {
	case 0:
		m_skinModelRender->ChangeModel("Assets/modelData/food/cheese.tkm");
		NowModelPath = "Assets/modelData/food/cheese.tkm";
		break;
	case 1:
		m_skinModelRender->ChangeModel("Assets/modelData/food/egg.tkm");
		NowModelPath = "Assets/modelData/food/egg.tkm";
		break;
	case 2:
		m_skinModelRender->ChangeModel("Assets/modelData/food/lettuce.tkm");
		NowModelPath = "Assets/modelData/food/lettuce.tkm";
		break;
	case 3:
		m_skinModelRender->ChangeModel("Assets/modelData/food/patty.tkm");
		NowModelPath = "Assets/modelData/food/patty.tkm";
		break;
	case 4:
		m_skinModelRender->ChangeModel("Assets/modelData/food/tomato.tkm");
		NowModelPath = "Assets/modelData/food/tomato.tkm";
		break;
	case 5:
		m_skinModelRender->ChangeModel("Assets/modelData/food/onion.tkm");
		NowModelPath = "Assets/modelData/food/onion.tkm";
		break;
	case 6:
		m_skinModelRender->ChangeModel("Assets/modelData/food/bacon.tkm");
		NowModelPath = "Assets/modelData/food/bacon.tkm";
		break;
	}
	
	m_skinModelRender->SetNewModel();
}

//キッチン上で別のモデルに差し替える用
void Guzai::ChangeModel(int& num)
{
	switch (num) {
	case 0:
		m_skinModelRender->ChangeModel("Assets/modelData/food/cheese_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/cheese_kitchen.tkm";
		break;
	case 1:
		m_skinModelRender->ChangeModel("Assets/modelData/food/egg_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/egg_kitchen.tkm";
		break;
	case 2:
		m_skinModelRender->ChangeModel("Assets/modelData/food/lettuce_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/lettuce_kitchen.tkm";
		break;
	case 3:
		m_skinModelRender->ChangeModel("Assets/modelData/food/patty_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/patty_kitchen.tkm";
		break;
	case 4:
		m_skinModelRender->ChangeModel("Assets/modelData/food/tomato_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/tomato_kitchen.tkm";
		break;
	case 5:
		m_skinModelRender->ChangeModel("Assets/modelData/food/onion_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/onion_kitchen.tkm";
		break;
	case 6:
		m_skinModelRender->ChangeModel("Assets/modelData/food/bacon_kitchen.tkm");
		NowModelPath = "Assets/modelData/food/bacon_kitchen.tkm";
		break;
	default:
		break;
	}
	
	m_skinModelRender->SetNewModel();
}

bool Guzai::Start()
{
	pl01 = FindGO<Player>("player01");
	pl02 = FindGO<Player>("player02");
	ki01 = FindGO<Kitchen>("kitchen01");
	ki02 = FindGO<Kitchen>("kitchen02");
	playerGene = FindGO<PlayerGene>("playerGene");
	m_guzaiGene = FindGO<GuzaiGene>("guzaiGene");
	m_guzaiOkiba = FindGO<GuzaiOkiba>("GuzaiOkiba");
	m_trashCan[0] = FindGO<TrashCan>("trashcan01");
	m_trashCan[1] = FindGO<TrashCan>("trashcan02");

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/cheese.tkm",nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	

	std::random_device rnd;
	std::mt19937 mt(rnd());
	//具材を追加したので乱数が出力する値を変更
	//std::uniform_int_distribution<int> rand(0, 4);
	std::uniform_int_distribution<int> rand(0, 6);

	TypeNo = rand(mt);

	switch (TypeNo) {
	case 0:
		m_skinModelRender->ChangeModel("Assets/modelData/food/cheese.tkm");
		NowModelPath = "Assets/modelData/food/cheese.tkm";
		break;
	case 1:
		m_skinModelRender->ChangeModel("Assets/modelData/food/egg.tkm");
		NowModelPath = "Assets/modelData/food/egg.tkm";
		break;
	case 2:
		m_skinModelRender->ChangeModel("Assets/modelData/food/lettuce.tkm");
		NowModelPath = "Assets/modelData/food/lettuce.tkm";
		break;
	case 3:
		m_skinModelRender->ChangeModel("Assets/modelData/food/patty.tkm");
		NowModelPath = "Assets/modelData/food/patty.tkm";
		break;
	case 4:
		m_skinModelRender->ChangeModel("Assets/modelData/food/tomato.tkm");
		NowModelPath = "Assets/modelData/food/tomato.tkm";
		break;
	case 5:
		m_skinModelRender->ChangeModel("Assets/modelData/food/onion.tkm");
		NowModelPath = "Assets/modelData/food/onion.tkm";
		break;
	case 6:
		m_skinModelRender->ChangeModel("Assets/modelData/food/bacon.tkm");
		NowModelPath = "Assets/modelData/food/bacon.tkm";
		break;

	}


	m_skinModelRender->SetNewModel();

	return true;
}

void Guzai::GrabNPut()
{
	Vector3 plPos01 = pl01->GetPosition();
	Vector3 plPos02 = pl02->GetPosition();
	
	//Aボタンを押したとき、プレイヤーは何も持っていない　一定距離より近い位置にいる。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (pl01->have == 0 && guzai2Pl01 < 150.0f && targeted == true && put == 0) {
			GetGuzaiOkiba();
			state = 1;
			pl01->have = 1;
			
			whichPlayerGet = 1;


			playerGene->AddNoHavingDishCounter();
		}
	}
	if (g_pad[1]->IsTrigger(enButtonA)) {
		if (pl02->have == 0 && guzai2Pl02 < 150.0f && targeted == true && put == 0) {
			GetGuzaiOkiba();
			state = 1;
			pl02->have = 1;
			
			whichPlayerGet = 2;

			playerGene->AddNoHavingDishCounter();
		}
	}

	//持たれていたら具材の位置をプレイヤーの上にする。
	if (state == 1) {
		if (whichPlayerGet == 1) {
			//plPos01.y += 100.0f;
			//SetPosition(plPos01);
			//具材の位置をプレイヤーの少し前にする。
			Vector3 pl01MSpeed = pl01->GetNormalMoveSpeed();
			pl01MSpeed *= 90.0f;
			plPos01 += pl01MSpeed;
			SetPosition(plPos01);

			//具材置き場に置いた後でもまた近づくとダミーが出るようにする。
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			if (targetDummy01 != nullptr) {
				DeleteGO(targetDummy01);
				isSetTargetDummy = false;
				decrementTime = holdTime;
				whichPlayerTargetMe = 0;
			}
		}
		if (whichPlayerGet == 2) {
			//plPos02.y += 100.0f;
			//SetPosition(plPos02);
			Vector3 pl02MSpeed = pl02->GetNormalMoveSpeed();
			pl02MSpeed *= 90.0f;
			plPos02 += pl02MSpeed;
			SetPosition(plPos02);

			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {

				DeleteGO(targetDummy02);
				isSetTargetDummy = false;
				decrementTime = holdTime;
				whichPlayerTargetMe = 0;

			}
		}
	}

	//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
	//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
	if (g_pad[0]->IsTrigger(enButtonA) && m_cooking == true) {
		if (state == 1 && kit2Pl01 < 100.0f) {
			Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
			//キッチンに置いた具材の種類をプレイヤー側に保存
			pl01->GuzaiNo[ki01->GetStackNum()] = TypeNo;
			pl01->have = 0;

			ki01->PlusStack();

			//ターゲティングしていた具材を運んでいる最中は別の具材をターゲティングしたくないため、ここで初期化。
			targeted = false;
			pl01->SetTarget(targeted);
			isSetTargetDummy = false;
			decrementTime = holdTime;

			//具材の情報を消す。
			DeleteGO(this);
		}
	}
	if (g_pad[1]->IsTrigger(enButtonA) && m_cooking == true) {
		if (state == 1 && kit2Pl02 < 100.0f) {
			Kitchen* ki02 = FindGO<Kitchen>("kitchen02");

			pl02->GuzaiNo[ki02->GetStackNum()] = TypeNo;
			pl02->have = 0;
			
			ki02->PlusStack();
			
			targeted = false;
			pl02->SetTarget(targeted);
			isSetTargetDummy = false;
			decrementTime = holdTime;

			DeleteGO(this);
		}
	}
}

void Guzai::TargetingNPopDummy()
{
		//具材との距離が一定以下　で　プレイヤーは何もロックしていなかったら。
		//近くの具材をターゲットし、プレイヤーのターゲット状態をTRUEに。
		if (guzai2Pl01 < TargetRangeNear && pl01->GetTargetState() == false && !targeted && put == 0) {
			whichPlayerTargetMe = 1;
			targeted = true;
			pl01->SetTarget(targeted);
		}
		if (guzai2Pl02 < TargetRangeNear && pl02->GetTargetState() == false && !targeted && put == 0) {
			whichPlayerTargetMe = 2;
			targeted = true;
			pl02->SetTarget(targeted);
		}

		//ターゲットした具材がダミーを出していなかったら。
		//少し大きいダミーを具材と被るように出す。（色は後で真っ黒にするのでなんでもいい）
		if (whichPlayerTargetMe != 0 && isSetTargetDummy == false) {
			if (whichPlayerTargetMe == 1) {
				SkinModelRender* targetDummyOnGuzai01 = NewGO<SkinModelRender>(1, "targetdummy01");
				targetDummyOnGuzai01->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
				targetDummyOnGuzai01->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				targetDummyOnGuzai01->SetFrontCulling("FrontCulling");
				targetDummyOnGuzai01->SetPosition(m_position);
				targetDummyOnGuzai01->SetScale(m_dummyScale);
				isSetTargetDummy = true;
			}
			if (whichPlayerTargetMe == 2) {
				SkinModelRender* targetDummyOnGuzai02 = NewGO<SkinModelRender>(1, "targetdummy02");
				targetDummyOnGuzai02->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
				targetDummyOnGuzai02->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				targetDummyOnGuzai02->SetFrontCulling("FrontCulling");
				targetDummyOnGuzai02->SetPosition(m_position);
				targetDummyOnGuzai02->SetScale(m_dummyScale);
				isSetTargetDummy = true;
			}
		}

		//ここでターゲットしていた具材から一定以上離れたら
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl01 >= TargetRangeFar && pl01->GetTargetState() == true && targeted == true) {
			if (whichPlayerTargetMe == 1) {
				SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
				if (targetDummy01 != nullptr) {
					decrementTime--;
					if (decrementTime == 0) {
						DeleteGO(targetDummy01);
						targeted = false;
						pl01->SetTarget(targeted);
						isSetTargetDummy = false;
						decrementTime = holdTime;
						whichPlayerTargetMe = 0;
					}
				}
			}
		}
		if (guzai2Pl02 >= TargetRangeFar && pl02->GetTargetState() == true && targeted == true) {
			if (whichPlayerTargetMe == 2) {
				SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
				if (targetDummy02 != nullptr) {
					decrementTime--;
					if (decrementTime == 0) {
						DeleteGO(targetDummy02);
						targeted = false;
						pl02->SetTarget(targeted);
						isSetTargetDummy = false;
						decrementTime = holdTime;
						whichPlayerTargetMe = 0;
					}
				}
			}
		}
}
void Guzai::SetGuzaiOkiba()
{
	//1P側の処理

	//具材がプレイヤーに持たれているときに、Bボタンが押されたら…
	if (g_pad[0]->IsTrigger(enButtonA) && state == 1 && whichPlayerGet == 1) {

		//1P側の具材置き場の番号は4～7なので、その範囲で調べる。
		for (int i = 4; i < 8; i++) {

			//具材置き場にセット可能かどうか確認する。
			if (m_guzaiOkiba->FindKitchenSet(i) == true && m_guzaiOkiba->FindGuzaiSet(i) == false && m_guzaiOkibaSet == false) {

				//セット可能ならば具材置き場にセットされたことを伝え、自身の座標をセットされた具材置き場にする。
				m_guzaiOkiba->GuzaiSet(i, true);
				m_position = m_guzaiOkiba->GetKitchenPos(i);
				if (m_cooking == true) {
					m_position.y += 60.0f;
				}
				m_guzaiOkibaSet = true;
				m_setKitchenNum = i;
				//プレイヤーが何も持っていない状態にする。
				pl01->have = 0;
				targeted = false;
				pl01->SetTarget(targeted);
				isSetTargetDummy = false;
				decrementTime = holdTime;
				state = 0;
			}
		}
	}
	//2P側の処理 1Pとほぼ同じ
	if (g_pad[1]->IsTrigger(enButtonA) && state == 1 && whichPlayerGet == 2) {
		//2P側の具材置き場の番号は0～4なので、その範囲で調べる。
		for (int i = 0; i < 4; i++) {
			
			if (m_guzaiOkiba->FindKitchenSet(i) == true && m_guzaiOkiba->FindGuzaiSet(i) == false && m_guzaiOkibaSet == false) {
			
				m_guzaiOkiba->GuzaiSet(i, true);
				m_position = m_guzaiOkiba->GetKitchenPos(i);
				if (m_cooking == true) {
					m_position.y += 60.0f;
				}
			
				m_guzaiOkibaSet = true;
				m_setKitchenNum = i;

				pl02->have = 0;
				targeted = false;
				pl02->SetTarget(targeted);
				isSetTargetDummy = false;
				decrementTime = holdTime;
				state = 0;
			}
		}
	}
}

void Guzai::GetGuzaiOkiba()
{
	//具材置き場にセットされていたら…
	if (m_guzaiOkibaSet == true) {
		//セットされていた具材置き場に取り出されたことを伝える。
		m_guzaiOkiba->GuzaiSet(m_setKitchenNum, false);
		//そして自身が取り出されたことにする。
		m_guzaiOkibaSet = false;
		m_setKitchenNum = 9;

		canPutOnGuzaiOkiba = false;

		playerGene->MinusNoHavingDishCounter();
	}
}

void Guzai::Cooking()
{
	//自身が具材置き場にセットされていて、調理されておらず、ダミーを出しているとき。
	if (m_guzaiOkibaSet == true && m_cooking == false && isSetTargetDummy == true) {
		//1P側の処理
		//1P側のBボタンが押されていて自身のセット場所が1P側だった場合…
		if (g_pad[0]->IsPress(enButtonB) && m_setKitchenNum >= 4) {
			//押している時間をインクリメント
			m_hold01++;
			//調理完了時間まで押されたら…
			if (m_hold01 > m_cookingTime) {
				//調理後のモデルに変更。
				ChangeModel(TypeNo);
				m_cooking = true;
				m_position.y += 50.0f;
				//そのままだと調理前のダミーが残るので一旦ダミーを消す。
				SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
				if (targetDummy01 != nullptr) {
					DeleteGO(targetDummy01);
					targeted = false;
					pl01->SetTarget(targeted);
					isSetTargetDummy = false;
					whichPlayerTargetMe = 0;
				}
			}
		}
		else {
			//ボタンを離したらタイマーは0に戻る。
			m_hold01 = 0;
		}

		//2P側の処理
		if (g_pad[1]->IsPress(enButtonB) && m_setKitchenNum < 4) {
			
			m_hold02++;
			
			if (m_hold02 > m_cookingTime) {
				
				ChangeModel(TypeNo);
				m_cooking = true;
				m_position.y += 50.0f;
				
				SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
				if (targetDummy02 != nullptr) {
					DeleteGO(targetDummy02);

					targeted = false;
					pl02->SetTarget(targeted);
					isSetTargetDummy = false;
					whichPlayerTargetMe = 0;
				}
			}
		}
		else {
			
			m_hold02 = 0;
		}
	}
}

void Guzai::SetOnTrashCan() {
	if (g_pad[0]->IsTrigger(enButtonA) 
		&& state == 1
		&& m_trashCan[0]->GetCanTrash()) {
		isSetOnTrashCan = true;
	}
	if (isSetOnTrashCan == true) {
		decrementTime--;
		if (decrementTime <= 10) {
			DeleteGO(this);

			pl01->have = 0;
			targeted = false;
			pl01->SetTarget(targeted);
		}
	}

	if (g_pad[1]->IsTrigger(enButtonA) 
		&& state == 1
		&& m_trashCan[1]->GetCanTrash()) {
		isSetOnTrashCan = true;
	}
	if (isSetOnTrashCan == true) {
		decrementTime--;
		if (decrementTime <= 10) {
			DeleteGO(this);

			pl02->have = 0;
			targeted = false;
			pl02->SetTarget(targeted);
		}
	}
}

void Guzai::Update()
{
	//プレイヤー生成中はUpdate関数をスルー
	if (playerGene->GetPlayerGeneState() == true) {
		return;
	}

	Vector3 plPos01 = pl01->GetPosition();
	Vector3 plPos02 = pl02->GetPosition();
	Vector3 kitchen01Pos = ki01->GetKitchenPos();
	Vector3 kitchen02Pos = ki02->GetKitchenPos();

	//具材からプレイヤーまでの距離
	guzai2Pl01 = (m_position.x - plPos01.x) * (m_position.x - plPos01.x) + (m_position.y - plPos01.y) * (m_position.y - plPos01.y) + (m_position.z - plPos01.z) * (m_position.z - plPos01.z);
	guzai2Pl01 = sqrt(guzai2Pl01);

	//キッチンからプレイヤーの距離
	kit2Pl01 = (kitchen01Pos.x - plPos01.x) * (kitchen01Pos.x - plPos01.x) + (kitchen01Pos.y - plPos01.y) * (kitchen01Pos.y - plPos01.y) + (kitchen01Pos.z - plPos01.z) * (kitchen01Pos.z - plPos01.z);
	kit2Pl01 = sqrt(kit2Pl01);

	//具材からプレイヤーへの距離
	guzai2Pl02 = (m_position.x - plPos02.x) * (m_position.x - plPos02.x) + (m_position.y - plPos02.y) * (m_position.y - plPos02.y) + (m_position.z - plPos02.z) * (m_position.z - plPos02.z);
	guzai2Pl02 = sqrt(guzai2Pl02);

	//キッチンからプレイヤーへの距離
	kit2Pl02 = (kitchen02Pos.x - plPos02.x) * (kitchen02Pos.x - plPos02.x) + (kitchen02Pos.y - plPos02.y) * (kitchen02Pos.y - plPos02.y) + (kitchen02Pos.z - plPos02.z) * (kitchen02Pos.z - plPos02.z);
	kit2Pl02 = sqrt(kit2Pl02);

	//トマトとオニオン以外は調理しないでよい。
	if (TypeNo != 4 && TypeNo != 5) {
		m_cooking = true;
	}

	TargetingNPopDummy();

	GrabNPut();

	if (canPutOnGuzaiOkiba) {
		SetGuzaiOkiba();
	}

	canPutOnGuzaiOkiba = true;

	Cooking();

	SetOnTrashCan();

	//ダミーを動かすよう
	if (isSetTargetDummy == true && state != 1) {
		if (whichPlayerTargetMe == 1) {
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			if (targetDummy01 != nullptr) {
				//調理後のチーズのみ、そのままだとダミーを出したときモデルが重なってしまうので少しだけy座標を上げる。
				//if (m_cooking == true && TypeNo == 0) {
				//	Vector3 SetPos = m_position;
				//	SetPos.y += 60.0f;
				//	targetDummy01->SetPosition(SetPos);
				//}

				//具材置き場置いてあるときはダミーの位置も上げる
				if (m_guzaiOkibaSet == true) {
					Vector3 SetPos = m_position;
					SetPos.y += 50.0f;
					targetDummy01->SetPosition(SetPos);
				}
				else {
					targetDummy01->SetPosition(m_position);
				}

			}
		}
		if (whichPlayerTargetMe == 2) {
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {
				/*if (m_cooking == true && TypeNo == 0) {
					Vector3 SetPos = m_position;
					SetPos.y += 55.0f;
					targetDummy02->SetPosition(SetPos);
				}*/
				if(m_guzaiOkibaSet == true){
					Vector3 SetPos = m_position;
					SetPos.y += 50.0f;
					targetDummy02->SetPosition(SetPos);
				}
				else {
					targetDummy02->SetPosition(m_position);
				}
			}
		}
	}

	//キッチンに載ってるときちょっと回してみた
	if (put == 1) {

		//回転処理
		angle += 2.0f;
		if (angle > 360.0f) {
			angle = 0.0f;
		}
		m_rotation.SetRotationDeg(Vector3::AxisY, angle);

	}


	m_skinModelRender->SetRotation(m_rotation);
	m_skinModelRender->SetScale(m_scale);

	//具材置き場に置かれているときの位置調整
	if (m_guzaiOkibaSet == true) {
		Vector3 SetPos = m_position;
		SetPos.y += 50.0f;
		m_skinModelRender->SetPosition(SetPos);
	}
	else {
		m_skinModelRender->SetPosition(m_position);
	}
}