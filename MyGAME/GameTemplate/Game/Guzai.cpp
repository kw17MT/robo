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
#include "SoundSource.h"
#include "effect/Effect.h"
#include "Meter.h"
#include "GameDirector.h"
#include<random>
#include <ctime>
#include <cstdlib>

#include "Game.h"

namespace
{
	const float MOVESPEED = 130.0f;
}

Guzai::Guzai()
{
}

Guzai::~Guzai()
{
	DeleteGO(m_skinModelRender);
	
	//ダミーを消去
	if (targetDummy01 != nullptr) {
		DeleteGO(targetDummy01);
	}
	if (targetDummy02 != nullptr) {
		DeleteGO(targetDummy02);
	}

	if (m_soundFlag01 == true || m_soundFlag02) {
		DeleteGO(m_cookingSe);
		DeleteGO(m_meter);
	}
	
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
	
	
	//初めてプレイヤーに持たれる時
	//Aボタンを押したとき、プレイヤーは何も持っていない、一定距離より近い位置にいる、自分はターゲットされているか、一度でも置かれていないか。
	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (pl01->have == 0 && guzai2Pl01 < 150.0f && targeted == true && put == 0 && ki01->GetKitchenCooking() == false) {
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/poka01.wav", false);
			se->SetVolume(3.0f);
			se->Play(false);
			GetGuzaiOkiba();
			//もたれた！
			state = 1;
			//Player1は具材をもっている！
			pl01->have = 1;
			//自分はどっちのプレイヤーに持たれたか
			whichPlayerGet = 1;
			//空の皿の数を1増やす
			playerGene->AddNoHavingDishCounter();
		}
	}
	if (g_pad[1]->IsTrigger(enButtonA)) {
		if (pl02->have == 0 && guzai2Pl02 < 150.0f && targeted == true && put == 0) {
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/poka01.wav", false);
			se->SetVolume(3.0f);
			se->Play(false);
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
			//具材の位置をプレイヤーの少し前にする。
			Vector3 pl01MSpeed = pl01->GetNormalMoveSpeed();
			pl01MSpeed *= 90.0f;
			plPos01 += pl01MSpeed;
			plPos01.y += 50.0f;
			SetPosition(plPos01);

			//具材置き場に置いた後でもまた近づくとダミーが出るようにする。
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
			plPos02.y += 50.0f;
			SetPosition(plPos02);

			if (targetDummy02 != nullptr) {

				DeleteGO(targetDummy02);
				isSetTargetDummy = false;
				decrementTime = holdTime;
				whichPlayerTargetMe = 0;
			}
		}
	}

	//ここはキッチンに置く処理
	//Aボタンを押してその具材が調理されているとき（する必要がない時）
	if (g_pad[0]->IsTrigger(enButtonA) && m_isCooked == true && whichPlayerGet == 1) {
		//自分は持たれているか、距離は一定以内か、一度キッチンに置かれていないか。←pl01->have = 0を回避するため必要
		if (state == 1 && kit2Pl01 < 100.0f && returned == false) {
			ChangeModel(TypeNo);
			//卵やった時少し小さく
			if (TypeNo == 1) {
				m_scale = { 0.7f,1.0f,0.7f };
			}
			Kitchen* ki01 = FindGO<Kitchen>("kitchen01");
			//キッチンに置いた具材の種類をプレイヤー側に保存
			pl01->GuzaiNo[ki01->GetStackNum()] = TypeNo;
			//プレイヤーは何も持っていない
			pl01->have = 0;
			//積んだ層数を1足す
			ki01->PlusStack();
			//この具材はキッチンに置かれている
			put = 1;
			//自分は持たれていない
			state = 0;
			//ターゲティングしていた具材を運んでいる最中は別の具材をターゲティングしたくないため、ここで初期化。
			//プレイヤーはターゲットしていないにする。
			targeted = false;
			pl01->SetTarget(targeted);
			//ダミーを出しているか→出していないにして次から出るようにする
			isSetTargetDummy = false;
			//キッチンのY座標＋60 * 積んだ具材数 分上げる。
			m_position = ki01->GetKitchenPos();
			m_position.y += ki01->GetStackNum() * 60;
			m_skinModelRender->SetPosition(m_position);

			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/poka02.wav", false);
			se->SetVolume(3.0f);
			se->Play(false);

			//キッチンにあるスタックした具材の一覧にこの具材を追加。
			ki01->RegistStackedGuzai(this);

			//同じフレームで取れないようにする。
			ki01->ChangeGrabState(false);
		}
	}
	if (g_pad[1]->IsTrigger(enButtonA) && m_isCooked == true && whichPlayerGet == 2) {
		if (state == 1 && kit2Pl02 < 100.0f && returned == false) {
			ChangeModel(TypeNo);
			//卵やった時少し小さく
			if (TypeNo == 1) {
				m_scale = { 0.7f,1.0f,0.7f };
			}
			Kitchen* ki02 = FindGO<Kitchen>("kitchen02");
			//キッチンに置いた具材の種類をプレイヤー側に保存
			pl02->GuzaiNo[ki02->GetStackNum()] = TypeNo;
			//プレイヤーは何も持っていない
			pl02->have = 0;
			//積んだ層数を1足す
			ki02->PlusStack();
			//この具材は置かれている
			put = 1;
			//自分は持たれていない
			state = 0;
			//ターゲティングしていた具材を運んでいる最中は別の具材をターゲティングしたくないため、ここで初期化。
			//プレイヤーはターゲットしていないにする。
			targeted = false;
			pl02->SetTarget(targeted);
			//ダミーを出しているか→出していないにして次から出るようにする
			isSetTargetDummy = false;
			//キッチンのY座標＋60 * 積んだ具材数 分上げる。
			m_position = ki02->GetKitchenPos();
			m_position.y += ki02->GetStackNum() * 60;
			m_skinModelRender->SetPosition(m_position);

			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/poka02.wav", false);
			se->SetVolume(3.0f);
			se->Play(false);

			//キッチンにあるスタックした具材の一覧にこの具材を追加。
			ki02->RegistStackedGuzai(this);

			//同じフレームで取れないようにする。
			ki02->ChangeGrabState(false);
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
				//SkinModelRender* targetDummyOnGuzai01 = NewGO<SkinModelRender>(1, "targetdummy01");
				targetDummy01 = NewGO<SkinModelRender>(1, "targetdummy01");
				targetDummy01->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
				targetDummy01->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				targetDummy01->SetFrontCulling("FrontCulling");
				targetDummy01->SetPosition(m_position);
				targetDummy01->SetScale(m_dummyScale);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/select07.wav", false);
				se->SetVolume(0.2f);
				se->Play(false);
				isSetTargetDummy = true;
			}
			if (whichPlayerTargetMe == 2) {
				//SkinModelRender* targetDummyOnGuzai02 = NewGO<SkinModelRender>(1, "targetdummy02");
				targetDummy02 = NewGO<SkinModelRender>(1, "targetdummy02");
				targetDummy02->Init(NowModelPath, nullptr, enModelUpAxisZ, m_position);
				targetDummy02->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
				targetDummy02->SetFrontCulling("FrontCulling");
				targetDummy02->SetPosition(m_position);
				targetDummy02->SetScale(m_dummyScale);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/select07.wav", false);
				se->SetVolume(0.2f);
				se->Play(false);
				isSetTargetDummy = true;
			}
		}

		//ここでターゲットしていた具材から一定以上離れたら
		//ダミーを消して、プレイヤー側のTargetingStateとtargetedを元の値に戻してやる。
		if (guzai2Pl01 >= TargetRangeFar && pl01->GetTargetState() == true && targeted == true) {
			if (whichPlayerTargetMe == 1) {
				//SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
				//targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
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
				//SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
				//targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
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

	//具材がプレイヤーに持たれているときに、Aボタンが押されたら…
	if (g_pad[0]->IsTrigger(enButtonA) && state == 1 && whichPlayerGet == 1) {

		//1P側の具材置き場の番号は4～7なので、その範囲で調べる。
		for (int i = 4; i < 8; i++) {

			//具材置き場にセット可能かどうか確認する。
			if (m_guzaiOkiba->FindKitchenSet(i) == true && m_guzaiOkiba->FindGuzaiSet(i) == false && m_guzaiOkibaSet == false) {

				//セット可能ならば具材置き場にセットされたことを伝え、自身の座標をセットされた具材置き場にする。
				m_guzaiOkiba->GuzaiSet(i, true);
				m_position = m_guzaiOkiba->GetKitchenPos(i);
				if (m_isCooked == true) {
					m_position.y += 60.0f;
				}
				m_guzaiOkibaSet = true;
				m_setKitchenNum = i;
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/poka02.wav", false);
				se->SetVolume(3.0f);
				se->Play(false);
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
				if (m_isCooked == true) {
					m_position.y += 60.0f;
				}
			
				m_guzaiOkibaSet = true;
				m_setKitchenNum = i;
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/poka02.wav", false);
				se->SetVolume(3.0f);
				se->Play(false);
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
	if (m_guzaiOkibaSet == true && m_isCooked == false && isSetTargetDummy == true) {
		//1P側の処理
		//1P側のBボタンが押されていて自身のセット場所が1P側だった場合…
		if (g_pad[0]->IsPress(enButtonB) && m_setKitchenNum >= 4 && pl01->have <= 0) {
			
			//押している時間をインクリメント
			m_hold01++;
			pl01->StopMove01(true);
			pl01->SetMoveSpeed({ 0.0f,0.0f,0.0f });
			//音が出ていなかったら。
			if (m_soundFlag01 == false) {
				//調理の進み具合を示すメーター
				m_meter = NewGO<Meter>(0);
				Vector3 pos = m_position;
				pos.x -= 350.0f;
				pos.y += 300.0f;
				pos.z += 20.0f;
				m_meter->SetPosition(pos);
				//音を鳴らす
				m_cookingSe = NewGO<CSoundSource>(0);
				m_cookingSe->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
				m_cookingSe->SetVolume(1.0f);
				m_cookingSe->Play(true);
				m_soundFlag01 = true;
			}
			//メーターを少しずつ
			Vector3 Scale = m_meter->GetScale();
			Scale.x -= 1.4f / 60.0f;
			m_meter->SetScale(Scale);
			if (m_hold01 % 10 == 0) {
				Vector3 pos = m_position;
				pos.x -= 100.0f;
				pos.y += 100.0f;
				
			}
			//調理完了時間まで押されたら…
			if (m_hold01 > m_cookingTime) {
				//調理後のモデルに変更。
				ChangeModel(TypeNo);
				m_isCooked = true;
				m_position.y += 50.0f;
				//そのままだと調理前のダミーが残るので一旦ダミーを消す。
				//SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
				if (targetDummy01 != nullptr) {
					DeleteGO(targetDummy01);
					targeted = false;
					pl01->SetTarget(targeted);
					isSetTargetDummy = false;
					whichPlayerTargetMe = 0;
				}
				//音が出ていたら。
				if (m_soundFlag01 == true) {
					DeleteGO(m_meter);
					//音を消す。
					DeleteGO(m_cookingSe);
					m_soundFlag01 = false;
				}
				//動けるようにする。
				pl01->StopMove01(false);
			}
		}
		else {
			//ボタンを離したらタイマーは0に戻る。
			m_hold01 = 0;
			//動けるようにする。
			pl01->StopMove01(false);
			//音が出ていたら。
			if (m_soundFlag01 == true) {
				DeleteGO(m_meter);
				//音を消す。
				DeleteGO(m_cookingSe);
				m_soundFlag01 = false;
			}
		}

		//2P側の処理
		if (g_pad[1]->IsPress(enButtonB) && m_setKitchenNum < 4 && pl02->have <= 0) {
			
			m_hold02++;
			pl02->StopMove01(true);
			pl02->SetMoveSpeed({ 0.0f,0.0f,0.0f });
			////音が出ていなかったら。
			if (m_soundFlag02 == false) {
				m_meter = NewGO<Meter>(0);
				Vector3 pos = m_position;
				pos.x += 250.0f;
				pos.y += 300.0f;
				pos.z += 20.0f;
				m_meter->SetPosition(pos);
				//音を鳴らす
				m_cookingSe = NewGO<CSoundSource>(0);
				m_cookingSe->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
				m_cookingSe->SetVolume(1.0f);
				m_cookingSe->Play(true);
				m_soundFlag02 = true;
			}
			Vector3 Scale = m_meter->GetScale();
			Scale.x -= 1.4f / 60.0f;
			m_meter->SetScale(Scale);
			if (m_hold02 > m_cookingTime) {
				
				ChangeModel(TypeNo);
				m_isCooked = true;
				m_position.y += 50.0f;
				
				//SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
				if (targetDummy02 != nullptr) {
					DeleteGO(targetDummy02);

					targeted = false;
					pl02->SetTarget(targeted);
					isSetTargetDummy = false;
					whichPlayerTargetMe = 0;
				}
				//音が出ていたら。
				if (m_soundFlag02 == true) {
					//音を消す。
					DeleteGO(m_cookingSe);
					m_soundFlag02 = false;
					DeleteGO(m_meter);
				}
				pl02->StopMove01(false);
			}
		}
		else {
			pl02->StopMove01(false);
			m_hold02 = 0;
			//音が出ていたら。
			if (m_soundFlag02 == true) {
				//音を消す。
				DeleteGO(m_meter);
				DeleteGO(m_cookingSe);
				m_soundFlag02 = false;
			}
		}
	}
}

void Guzai::SetOnTrashCan() {
	
	
	if (whichPlayerGet == 1) {
		if (g_pad[0]->IsTrigger(enButtonA)					//Aボタンを押して
			&& state == 1									//この具材が持たれていて
			&& m_trashCan[0]->GetCanTrash()) {				//ゴミ箱は捨てる用意ができているか（距離的に）
			isSetOnTrashCan = true;							//ゴミ箱で捨てる準備
		}
		if (isSetOnTrashCan == true) {
			DeleteGO(this);
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/dumping.wav", false);
			se->SetVolume(2.0f);
			se->Play(false);
			pl01->have = 0;
			targeted = false;
			pl01->SetTarget(targeted);

			m_trashCan[0]->ChangeMovingState(true);
		}
	}
	if (whichPlayerGet == 2) {
		if (g_pad[1]->IsTrigger(enButtonA)
			&& state == 1
			&& m_trashCan[1]->GetCanTrash()) {
			isSetOnTrashCan = true;
		}
		if (isSetOnTrashCan == true) {
			DeleteGO(this);
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/dumping.wav", false);
			se->SetVolume(2.0f);
			se->Play(false);
			pl02->have = 0;
			targeted = false;
			pl02->SetTarget(targeted);
			m_trashCan[1]->ChangeMovingState(true);
		}
	}
}

void Guzai::Rotation()
{
	//キッチンにセットされているとき。
	if (put == 1) {

		//回転処理
		angle += 2.0f;
		if (angle > 360.0f) {
			angle = 0.0f;
		}
		m_rotation.SetRotationDeg(Vector3::AxisY, angle);

	}
	//キッチンにセットされていないとき。
	else
	{
		//プレイヤーが具材を持ったときの具材の追従回転。
		if (state == 1) {
			if (whichPlayerGet == 1) {
				//プレイヤーが向いている方向に回転するので、プレイヤーの移動速度を参照する。
				Vector3 pl01MSpeed = pl01->GetNormalMoveSpeed();
				angle = atan2(pl01MSpeed.x, pl01MSpeed.z);
				m_rotation.SetRotation(Vector3::AxisY, angle);
			}
			if (whichPlayerGet == 2) {
				Vector3 pl02MSpeed = pl02->GetNormalMoveSpeed();
				angle = atan2(pl02MSpeed.x, pl02MSpeed.z);
				m_rotation.SetRotation(Vector3::AxisY, angle);
			}
		}
		else {
			//プレイヤーが持っていないときは回転をリセット。
			m_rotation = Quaternion::Identity;
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
		m_isCooked = true;
	}

	TargetingNPopDummy();

	ki01->ChangeGrabState(true);
	ki02->ChangeGrabState(true);

	GrabNPut();

	if (canPutOnGuzaiOkiba) {
		SetGuzaiOkiba();
	}

	canPutOnGuzaiOkiba = true;

	Cooking();

	SetOnTrashCan();

	Rotation();

	//ダミーを動かすよう
	if (isSetTargetDummy == true && state != 1) {
		if (whichPlayerTargetMe == 1) {
			//SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
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
			//SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
			if (targetDummy02 != nullptr) {
				/*if (m_cooking == true && TypeNo == 0) {
					Vector3 SetPos = m_position;
					SetPos.y += 55.0f;
					targetDummy02->SetPosition(SetPos);
				}*/
				if (m_guzaiOkibaSet == true) {
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

	if (returned){
		if (state == 1) {
			if (whichPlayerGet == 1) {
				//具材の位置をプレイヤーの少し前にする。
				Vector3 pl01MSpeed = pl01->GetNormalMoveSpeed();
				pl01MSpeed *= 90.0f;
				plPos01 += pl01MSpeed;
				SetPosition(plPos01);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/putting_a_book2.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);

				//具材置き場に置いた後でもまた近づくとダミーが出るようにする。
				//SkinModelRender* targetDummy01 = FindGO<SkinModelRender>("targetdummy01");
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
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/putting_a_book2.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);

				//SkinModelRender* targetDummy02 = FindGO<SkinModelRender>("targetdummy02");
				if (targetDummy02 != nullptr) {

					DeleteGO(targetDummy02);
					isSetTargetDummy = false;
					decrementTime = holdTime;
					whichPlayerTargetMe = 0;

				}
			}
		}
		returned = false;
	}


	//キッチンに載ってるときちょっと回してみた
	//if (put == 1) {

	//	//回転処理
	//	angle += 2.0f;
	//	if (angle > 360.0f) {
	//		angle = 0.0f;
	//	}
	//	m_rotation.SetRotationDeg(Vector3::AxisY, angle);

	//}


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