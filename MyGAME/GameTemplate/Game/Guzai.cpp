#include "stdafx.h"
#include "Guzai.h"
#include "ModelRender.h"
#include "math.h"
#include "Kitchen.h"
#include "ObjectGene.h"
#include "SpriteRender.h"
#include <ctime>
#include <cstdlib>

#include "PathMove.h"

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

void Guzai::Move()
{
	Vector3 GuzaiPos = m_charaCon.GetPosition();

	//TODO 具材の移動。
	/*if (GuzaiNo == 1) {
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
			//TODO ここで具材を削除してる。
			if (GuzaiPos.z < -850.0f) {
				timer = 0;
				DeleteGO(this);
				Guzai* targetdummy01 = FindGO<Guzai>("targetdummy01");
				if (targetdummy01 != nullptr) {
					DeleteGO(targetdummy01);
				}
			}
			m_charaCon.Execute(moveSpeed, 1.0f);
		}
	}
	if (GuzaiNo == 2) {
		//持たれていない　且つ　一度も置かれていない
		if (state == 0 && put == 0) {
			Vector3 moveSpeed = { 0.0f,0.0f,0.0f };
			timer++;
			if (timer < 500) {
				moveSpeed.z = 2.0f;
			}
			if (timer >= 500 && timer < 600) {
				moveSpeed.x = -2.0f;
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
	}*/

	//持たれていない　且つ　一度も置かれていない
	if (state == 0 && put == 0) {
		//移動させる。
		m_charaCon.SetPosition(m_pathMove.get()->Move());
		//最後のポイントまで到達したら。
		if (m_pathMove.get()->GetIsFinalPoint())
		{
			//削除する。
			DeleteGO(this);
		}
	}
}

bool Guzai::Start()
{
	pl01 = FindGO<ModelRender>("player01");
	pl02 = FindGO<ModelRender>("player02");
	ki01 = FindGO<Kitchen>("kitchen01");
	ki02 = FindGO<Kitchen>("kitchen02");

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

	modeldata.m_expandConstantBuffer = &g_lig;
	modeldata.m_expandConstantBufferSize = sizeof(g_lig);

	modeldata.m_colorBufferFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;

	modeldata.m_modelUpAxis = enModelUpAxisY;

	model.Init(modeldata);

	Vector3 pos = { 0.0f,0.0f,-1000.0f };

	m_charaCon.Init(0.0f, 0.0f, pos);


	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_charaCon.GetPosition(), MOVESPEED, enNormalLane, GuzaiNo);

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
			Guzai* targetdummy01 = FindGO<Guzai>("targetdummy01");
			DeleteGO(targetdummy01);
		}

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[0]->IsTrigger(enButtonB)) {
			if (state == 1 && kit2Pl < 400.0f) {
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

				DeleteGO(this);
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
			Guzai* targetdummy02 = FindGO<Guzai>("targetdummy02");
			DeleteGO(targetdummy02);
		}

		//Bボタンを押してキッチンが近くにあったら、今積まれている数に応じておく場所を変える。
		//キッチン側のスタック数をインクリメント。キッチン側で具材をNewGO。
		if (g_pad[1]->IsTrigger(enButtonB)) {
			if (state == 1 && kit2Pl < 400.0f) {
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

				DeleteGO(this);
			}
		}
	}
}

void Guzai::TargetingNPopDummy()
{
	Vector3 GuzaiPos = m_charaCon.GetPosition();

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
			Guzai* targetdummyOnGuzai01 = NewGO<Guzai>(1, "targetdummy01");
			targetdummyOnGuzai01->modeldata.m_psEntryPointFunc = "FrontCulling";
			targetdummyOnGuzai01->SetPosition(GuzaiPos);
			targetdummyOnGuzai01->SetScale({ 1.1f,1.1f,1.1f });
			isSetTargetDummy = true;


			//画像をターゲットした具材の上に置きたいが、3D表示が必要になりそう。//////////////////////////////////////////////
			/*sp01 = NewGO<SpriteRender>(3);
			sp01->Init("Assets/Image/burger_tomato.dds", 128, 256);
			Vector3 spritePos = m_charaCon.GetPosition();
			std::swap(spritePos.z, spritePos.y);
			spritePos.z = 0.0f;
			sp01->SetPosition(spritePos);*/
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		//ここで一定以上離れてプレイヤーは何かしらターゲットしていたら（if文の後ろの条件は別の具材に影響を与えるのを防ぐため。
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl >= TargetRangeFar && pl01->GetTargetState() == true && targeted == true) {
			Guzai* targetdummy01 = FindGO<Guzai>("targetdummy01");
			if (targetdummy01 != nullptr) {
				decrementTime--;
				if (decrementTime == 0) {
					DeleteGO(targetdummy01);
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
			Guzai* targetdummyOnGuzai02 = NewGO<Guzai>(1, "targetdummy02");
			targetdummyOnGuzai02->modeldata.m_psEntryPointFunc = "FrontCulling";
			targetdummyOnGuzai02->SetPosition(GuzaiPos);
			targetdummyOnGuzai02->SetScale({ 1.1f,1.1f,1.1f });
			isSetTargetDummy = true;
		}

		//ここで一定以上離れてプレイヤーは何かしらターゲットしていたら（if文の後ろの条件は別の具材に影響を与えるのを防ぐため。
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl >= TargetRangeFar && pl02->GetTargetState() == true && targeted == true) {
			Guzai* targetdummy02 = FindGO<Guzai>("targetdummy02");
			if (targetdummy02 != nullptr) {
				decrementTime--;
				if (decrementTime == 0) {
					DeleteGO(targetdummy02);
					targeted = false;
					pl02->SetTarget(targeted);
					isSetTargetDummy = false;
					decrementTime = holdTime;
				}
			}
		}
	}
}

//ターゲットした具材の上に画像を置きたいが3D表示が必要になりそう。////////////////////////////////////////////////
//void Guzai::PopTargetingIcon()
//{
//	//ターゲットダミーと一緒に出現させたいため。
//	if (isSetTargetDummy == true) {
//
//		Vector3 spritePos = m_charaCon.GetPosition();
//		spritePos *= -1.0f;
//		std::swap(spritePos.z, spritePos.y);
//		spritePos.z = 0.0f;
//		sp01->SetPosition(spritePos);
//		//sp01->SetPosition(m_charaCon.GetPosition());
//	}
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Guzai::Update()
{
	if (GuzaiNo == 1) {
		Vector3 plPos = pl01->GetPosition();
		Vector3 GuzaiPos = m_charaCon.GetPosition();
		Vector3 kitchen01Pos = ki01->GetKitchenPos();

		//具材からプレイヤーまでの距離
		guzai2Pl = (GuzaiPos.x - plPos.x) * (GuzaiPos.x - plPos.x) + (GuzaiPos.y - plPos.y) * (GuzaiPos.y - plPos.y) + (GuzaiPos.z - plPos.z) * (GuzaiPos.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//キッチンからプレイヤーの距離
		kit2Pl = (kitchen01Pos.x - plPos.x) * (kitchen01Pos.x - plPos.x) + (kitchen01Pos.y - plPos.y) * (kitchen01Pos.y - plPos.y) + (kitchen01Pos.z - plPos.z) * (kitchen01Pos.z - plPos.z);
		kit2Pl = sqrt(kit2Pl);
		
		if (isSetFirstPos == false) {
			ObjectGene* gene01 = FindGO<ObjectGene>("gene01");
			SetPosition(gene01->GetPosition());

			isSetFirstPos = true;
		}

		TargetingNPopDummy();

		//PopTargetingIcon();

		GrabNPut();

		Move();
		
		//ダミーを動かすよう
		if (isSetTargetDummy == true && state != 1) {
			Guzai* targetdummy01 = FindGO<Guzai>("targetdummy01");
			if (targetdummy01 != nullptr) {
				targetdummy01->SetPosition(GuzaiPos);
			}
		}
	}

	if (GuzaiNo == 2) {
		Vector3 plPos = pl02->GetPosition();
		Vector3 GuzaiPos = m_charaCon.GetPosition();
		Vector3 kitchen02Pos = ki02->GetKitchenPos();

		//具材からプレイヤーへの距離
		guzai2Pl = (GuzaiPos.x - plPos.x) * (GuzaiPos.x - plPos.x) + (GuzaiPos.y - plPos.y) * (GuzaiPos.y - plPos.y) + (GuzaiPos.z - plPos.z) * (GuzaiPos.z - plPos.z);
		guzai2Pl = sqrt(guzai2Pl);

		//キッチンからプレイヤーへの距離
		kit2Pl = (kitchen02Pos.x - plPos.x) * (kitchen02Pos.x - plPos.x) + (kitchen02Pos.y - plPos.y) * (kitchen02Pos.y - plPos.y) + (kitchen02Pos.z - plPos.z) * (kitchen02Pos.z - plPos.z);
		kit2Pl = sqrt(kit2Pl);

		if (isSetFirstPos == false) {
			ObjectGene* gene02 = FindGO<ObjectGene>("gene02");
			SetPosition(gene02->GetPosition());
			isSetFirstPos = true;
		}

		TargetingNPopDummy();
		
		GrabNPut();

		Move();

		//ダミーを動かすよう
		if (isSetTargetDummy == true && state != 1) {
			Guzai* targetdummy02 = FindGO<Guzai>("targetdummy02");
			if (targetdummy02 != nullptr) {
				targetdummy02->SetPosition(GuzaiPos);
			}
		}
	}
	model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, GuzaiScale);
}