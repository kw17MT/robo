#include "stdafx.h"
#include "Guzai.h"
#include "math.h"
#include "Kitchen.h"
#include "GuzaiGene.h"
#include "PathMove.h"
#include "SkinModelRender.h"
#include "Player.h"
#include "PlayerGene.h"
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
		m_skinModelRender->ChangeModel("Assets/modelData/gu/cheese.tkm");
		NowModelPath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/egg.tkm");
		NowModelPath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/lettuce.tkm");
		NowModelPath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/patty.tkm");
		NowModelPath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		//モデル差し替え
		/*m_skinModelRender->ChangeModel("Assets/modelData/gu/tomato.tkm");
		NowModelPath = "Assets/modelData/gu/tomato.tkm";*/
		m_skinModelRender->ChangeModel("Assets/modelData/food/tomato.tkm");
		/*Vector3*/ m_scale = { 2.0f,2.0f,2.0f };
		m_skinModelRender->SetScale(m_scale);
		NowModelPath = "Assets/modelData/food/tomato.tkm";
		break;
	}
	
	m_skinModelRender->SetNewModel();
}

void Guzai::Move()
{
	

	////持たれていない　且つ　一度も置かれていない
	//if (state == 0 && put == 0) {
	//	//移動させる。
	//	SetPosition(m_pathMove.get()->Move());
	//	//最後のポイントまで到達したら。
	//	if (m_pathMove.get()->GetIsFinalPoint())
	//	{
	//		//削除する。
	//		DeleteGO(this);
	//	}
	//}
}

bool Guzai::Start()
{
	pl01 = FindGO<Player>("player01");
	pl02 = FindGO<Player>("player02");
	ki01 = FindGO<Kitchen>("kitchen01");
	ki02 = FindGO<Kitchen>("kitchen02");
	playerGene = FindGO<PlayerGene>("playerGene");
	m_guzaiGene = FindGO<GuzaiGene>("guzaiGene");

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/gu/cheese.tkm",nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	

	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<int> rand(0, 4);

	TypeNo = rand(mt);

	switch (TypeNo) {
	case 0:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/cheese.tkm");
		NowModelPath = "Assets/modelData/gu/cheese.tkm";
		break;
	case 1:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/egg.tkm");
		NowModelPath = "Assets/modelData/gu/egg.tkm";
		break;
	case 2:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/lettuce.tkm");
		NowModelPath = "Assets/modelData/gu/lettuce.tkm";
		break;
	case 3:
		m_skinModelRender->ChangeModel("Assets/modelData/gu/patty.tkm");
		NowModelPath = "Assets/modelData/gu/patty.tkm";
		break;
	case 4:
		//モデル差し替え
		/*m_skinModelRender->ChangeModel("Assets/modelData/gu/tomato.tkm");
		NowModelPath = "Assets/modelData/gu/tomato.tkm";*/
		m_skinModelRender->ChangeModel("Assets/modelData/food/tomato.tkm");
		/*Vector3 */m_scale = { 2.0f,2.0f,2.0f };
		m_skinModelRender->SetScale(m_scale);
		NowModelPath = "Assets/modelData/food/tomato.tkm";
		break;
	}

	m_skinModelRender->SetNewModel();

	/*m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_position, MOVESPEED, enNormalLane, GuzaiNo);*/

	return true;
}

void Guzai::GrabNPut()
{
	Vector3 plPos01 = pl01->GetPosition();
	Vector3 plPos02 = pl02->GetPosition();
	
	//Aボタンを押したとき、プレイヤーは何も持っていない　一定距離より近い位置にいる。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (pl01->have == 0 && guzai2Pl01 < 150.0f && targeted == true) {
			state = 1;
			pl01->have = 1;
			whichPlayerGet = 1;

			//どこからとったか生成器に連絡する、両プレイヤーで共有
			m_guzaiGene->RegisterEmptyDishNumber(dishNumber);
			//フィールドからいくつ具材がとられているか数える
			m_guzaiGene->AddEmptyDishCounter();
		}
	}
	if (g_pad[1]->IsTrigger(enButtonA)) {
		if (pl02->have == 0 && guzai2Pl02 < 150.0f && targeted == true) {
			state = 1;
			pl02->have = 1;
			whichPlayerGet = 2;

			m_guzaiGene->RegisterEmptyDishNumber(dishNumber);
			m_guzaiGene->AddEmptyDishCounter();
		}
	}

	//持たれていたら具材の位置をプレイヤーの上にする。
	if (state == 1) {
		if (whichPlayerGet == 1) {
			plPos01.y += 100.0f;
			SetPosition(plPos01);

			//ターゲット用のダミーを消す。
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			m_scale -= expantionRate;
			DeleteGO(targetDummy01);

			

		}
		if (whichPlayerGet == 2) {
			plPos02.y += 100.0f;
			SetPosition(plPos02);

			//ターゲット用のダミーを消す。
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			m_scale -= expantionRate;
			DeleteGO(targetDummy02);

			
		}
	}

	//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
	//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
	if (g_pad[0]->IsTrigger(enButtonB)) {
		if (state == 1 && kit2Pl01 < 100.0f) {
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
	if (g_pad[1]->IsTrigger(enButtonB)) {
		if (state == 1 && kit2Pl02 < 100.0f) {
			Kitchen* ki02 = FindGO<Kitchen>("kitchen02");

			pl02->GuzaiNo[ki02->GetStackNum()] = TypeNo;
			ki02->PlusStack();

			pl02->have = 0;

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
		if (guzai2Pl01 < TargetRangeNear && pl01->GetTargetState() == false && !targeted) {
			whichPlayerTargetMe = 1;
			targeted = true;
			pl01->SetTarget(targeted);
		}
		if (guzai2Pl02 < TargetRangeNear && pl02->GetTargetState() == false && !targeted) {
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
				targetDummyOnGuzai01->InitShader("Assets/shader/model.fx", "VSMain", "FrontCulling", DXGI_FORMAT_R32G32B32A32_FLOAT);
				//targetdummyOnGuzai01->modeldata.m_psEntryPointFunc = "FrontCulling";
				targetDummyOnGuzai01->SetPosition(m_position);
				m_scale.x += 0.3f; m_scale.y += 0.3f; m_scale.z += 0.3f;
				targetDummyOnGuzai01->SetScale(m_scale/*{ 1.3f,1.3f,1.3f }*/);

				isSetTargetDummy = true;
			}
			if (whichPlayerTargetMe == 2) {
				SkinModelRender* targetDummyOnGuzai02 = NewGO<SkinModelRender>(1, "targetdummy02");
				targetDummyOnGuzai02->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
				targetDummyOnGuzai02->InitShader("Assets/shader/model.fx", "VSMain", "FrontCulling", DXGI_FORMAT_R32G32B32A32_FLOAT);
				//targetdummyOnGuzai02->modeldata.m_psEntryPointFunc = "FrontCulling";
				targetDummyOnGuzai02->SetPosition(m_position);
				m_scale.x += 0.3f; m_scale.y += 0.3f; m_scale.z += 0.3f;
				targetDummyOnGuzai02->SetScale({ m_scale/*1.3f,1.3f,1.3f*/ });
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
						m_scale -= expantionRate;
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
						m_scale -= expantionRate;
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

	
	TargetingNPopDummy();

	GrabNPut();

	Move();

	//ダミーを動かすよう
	if (isSetTargetDummy == true && state != 1) {
		if (whichPlayerTargetMe == 1) {
			SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
			if (targetDummy01 != nullptr) {
				targetDummy01->SetPosition(m_position);
			}
		}
		if (whichPlayerTargetMe == 2) {
			SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {
				targetDummy02->SetPosition(m_position);
			}
		}
	}

	m_skinModelRender->SetPosition(m_position);
}