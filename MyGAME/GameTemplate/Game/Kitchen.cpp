#include "stdafx.h"
#include "Kitchen.h"
#include "Burger.h"
#include "Guzai.h"
#include "Counter.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "SoundSource.h"
#include "Meter.h"
#include "GuzaiGene.h"
#include "MenuTimer.h"

namespace
{
	const int PLAYER_NAME_SIZE = 9;
}

Kitchen::~Kitchen()
{
	DeleteGO(m_skinModelRender);
	
	//バーガーの消去
	if (bur != nullptr) {
		DeleteGO(bur);
	}
	
	//スタックした具材が残っていたら消去
	for (int i = 0; i < 5; i++) {
		if (StackedGuzai[i] != nullptr) {
			DeleteGO(StackedGuzai[i]);
		}
	}

	if (m_soundFlag01 == true || m_soundFlag02) {
		DeleteGO(m_soundSource);
		DeleteGO(m_meter);
	}
}

bool Kitchen::Start()
{
	//string型に変えてcharに変換するための準備をする。
	std::string endNo_string = std::to_string(m_kitchenNo);
	//不変箇所を定義
	char playerName[PLAYER_NAME_SIZE] = "player0";
	//末端番号だけを追加する
	strcat_s(playerName, endNo_string.c_str());
	
	//必要なデータの探索と確保
	m_player = FindGO<Player>(playerName);
	m_guzaiGene = FindGO<GuzaiGene>("GuzaiGene");

	//モデルデータの初期化
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/object/kitchen.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_skinModelRender->SetScale(m_scale);

	return true;
}

void Kitchen::ChangeModel(const char* filePath)
{
	m_skinModelRender->Init(filePath, nullptr, enModelUpAxisZ, m_position);
}

//積んでいる具材の全消去、それに関するパラメータの初期化
//BornBurger()でつかう
void Kitchen::Delete()
{
	//積んでいる具材の消去
	if (m_kitchenNo == 1) {
		for (int i = 0;i < stack; i++) {
			DeleteGO(StackedGuzai[i]);
			m_guzaiGene->SetGuzaiFlag(StackedGuzai[i]->GetGuziNo(), false);
		}

		Counter* co01 = FindGO<Counter>("counter00");
		co01->SetStackNum(stack);

		//積んでいる具材数を０で初期化
		stack = 0;
	}
	if (m_kitchenNo == 2) {
		for (int i = 0;i < stack; i++) {
			DeleteGO(StackedGuzai[i]);
			m_guzaiGene->SetGuzaiFlag(StackedGuzai[i]->GetGuziNo(), false);
		}
		Counter* co02 = FindGO<Counter>("counter01");
		co02->SetStackNum(stack);

		stack = 0;
	}
}

// Delayは必要
// ないとエラー
void Kitchen::BornBurger()
{
	Vector3 predistance = m_player->GetPosition() - m_position;
	float distance1 = predistance.Length();

	//具材を一つ以上積んでいて、Bボタンを長押し
	if (stack >= 1 && g_pad[m_kitchenNo]->IsPress(enButtonB) && m_deleteFlag == false && distance1 < 100.0f && m_player->have <= 0) {
		Delay--;
		//プレイヤーが動けないようにする。
		m_player->StopMove02(true);
		m_player->SetMoveSpeed({ 0.0f,0.0f,0.0f });
		if (m_soundFlag01 == false) {
			//調理の進み具合のメーターを表示
			m_meter = NewGO<Meter>(0);
			Vector3 pos = m_position;
			pos.x -= 200.0f;
			pos.y += 300.0f;
			pos.z += 100.0f;
			m_meter->SetPosition(pos);

			//音を鳴らす
			m_soundSource = NewGO<CSoundSource>(0);
			m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
			m_soundSource->SetVolume(2.0f);
			m_soundSource->Play(true);
			m_soundFlag01 = true;
		}
		//メーターを少しずつ減らす
		Vector3 scale = m_meter->GetScale();
		scale.x -= 1.4 / 60;
		m_meter->SetScale(scale);
		if (Delay == 0) {
			//音が出ていれば消す。
			if (m_soundFlag01 == true) {
				//メーターを消す
				DeleteGO(m_meter);
				DeleteGO(m_soundSource);
				m_soundFlag01 = false;
			}
			//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
			m_player->have = 1;
			//削除フラグを立てる。
			m_deleteFlag = true;
			Delay = 40;
		}
	}
	else {
		if (m_deleteFlag == false) {
			Delay = 60;
			m_player -> StopMove02(false);
		}

		//音が出ていれば消す。
		if (m_soundFlag01 == true) {
			//メーターを消す。
			DeleteGO(m_meter);
			DeleteGO(m_soundSource);
			m_soundFlag01 = false;
		}
	}
	//削除フラグが立っているとき…
	if (m_deleteFlag == true) {
		m_player->have = 1;
		Delay--;
		for (int i = 0; i < stack; i++) {
			m_slidePos = StackedGuzai[i]->GetPosition();
			//具材のy座標を(速度×積んでる段数)で下げる。
			m_slidePos.y -= 1.2f * i;
			StackedGuzai[i]->SetPosition(m_slidePos);
		}
		if (Delay == 0) {
			//キッチンについている具材を全部消去
			Delete();

			bur = NewGO<Burger>(0, "burger00");
			bur->SetBurgerNo(0);
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/thi-n.wav", false);
			se->SetVolume(2.0f);
			se->Play(false);

			m_player->StopMove02(false);
			m_deleteFlag = false;
			Delay = 60;
		}
	}
}

	//if (m_kitchenNo == 2) {
	//	if (stack >= 1 && g_pad[1]->IsPress(enButtonB) && m_deleteFlag == false && distance2 < 100.0f == m_player[1]->have <= 0) {
	//		Delay--;
	//		m_player[1]->StopMove02(true);
	//		m_player[1]->SetMoveSpeed({ 0.0f,0.0f,0.0f });
	//		if (m_soundFlag02 == false) {
	//			m_meter = NewGO<Meter>(0);
	//			Vector3 pos = m_position;
	//			pos.x += 100.0f;
	//			pos.y += 300.0f;
	//			pos.z += 100.0f;
	//			m_meter->SetPosition(pos);
	//			//音を鳴らす
	//			m_soundSource = NewGO<CSoundSource>(0);
	//			m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
	//			m_soundSource->SetVolume(2.0f);
	//			m_soundSource->Play(true);
	//			m_soundFlag02 = true;
	//		}
	//		Vector3 scale = m_meter->GetScale();
	//		scale.x -= 1.4 / 60;
	//		m_meter->SetScale(scale);
	//		if (Delay == 0) {
	//			//音が出ていれば消す。
	//			if (m_soundFlag02 == true) {
	//				DeleteGO(m_meter);
	//				DeleteGO(m_soundSource);
	//				m_soundFlag02 = false;
	//			}
	//			//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
	//			m_player[1]->have = 1;
	//			m_deleteFlag = true;
	//			Delay = 40;
	//		}
	//	}
	//	else {
	//		if (m_deleteFlag == false) {
	//			Delay = 60;
	//			//m_player[1]->StopMove02(false);
	//		}
	//		//音が出ていれば消す。
	//		if (m_soundFlag02 == true) {
	//			DeleteGO(m_meter);
	//			DeleteGO(m_soundSource);
	//			m_soundFlag02 = false;
	//		}
	//	}
	//	if (m_deleteFlag == true) {
	//		m_player[1]->have = 1;
	//		Delay--;
	//		for (int i = 0; i < stack; i++) {
	//			m_slidePos = StackedGuzai[i]->GetPosition();
	//			//具材のy座標を(速度×積んでる段数)で下げる。
	//			m_slidePos.y -= 1.2f * i;
	//			StackedGuzai[i]->SetPosition(m_slidePos);
	//		}
	//		if (Delay == 0) {
	//			//キッチンについている具材を全部消去
	//			Delete();
	//		
	//			bur = NewGO<Burger>(0, "burger01");
	//			bur->SetBurgerNo(1);
	//			//音を鳴らす
	//			CSoundSource* se = NewGO<CSoundSource>(0);
	//			se->Init(L"Assets/sound/thi-n.wav", false);
	//			se->SetVolume(2.0f);
	//			se->Play(false);

	//			m_player[1]->StopMove02(false);
	//			m_deleteFlag = false;
	//			Delay = 60;
	//			
	//		}
	//	}
	//}


void Kitchen::ClearNo()
{
	for (int i = 0;i < stack; i++) {
		m_player->GuzaiNo[i] = 9;
	}
}

void Kitchen::Update()
{
	if (m_player == nullptr) {
		//string型に変えてcharに変換するための準備をする。
		std::string endNo_string = std::to_string(m_kitchenNo);
		//不変箇所を定義
		char playerName[PLAYER_NAME_SIZE] = "player0";
		//末端番号だけを追加する
		strcat_s(playerName, endNo_string.c_str());

		//必要なデータの探索と確保
		m_player = FindGO<Player>(playerName);

		m_player = FindGO<Player>(playerName);
	}
	

	Vector3 predistance = m_player->GetPosition() - m_position;
	float distance = predistance.Length();

	//ここいる
	//取る処理
	//1層以上積まれていたらとれるようにする。
	if (stack >= 1 && distance <= 150.0f) {
		if (g_pad[0]->IsTrigger(enButtonA) && m_kitchenNo == 1 && canGrab == true && m_player->have <= 0) {
			//この具材は一回キッチンから帰ってきたか
			StackedGuzai[stack - 1]->SetReturnedState(true);
			//持たれているか
			StackedGuzai[stack - 1]->SetisHadState(true);
			//どちらのプレイヤーに持たれているか
			StackedGuzai[stack - 1]->SetWhichPlayerGet(1);
			//一番上だった具材の番号を９にして何も入っていない状態にする。
			m_player->ClearLatestGuzaiNo(stack - 1);
			//この具材はキッチンに置かれているか
			StackedGuzai[stack - 1]->SetPutState(false);
			//現在のスタック数、次のスタック番号を1減らす
			stack--;
			//プレイヤーは具材を持っている状態にする。
			m_player->have = 1;
		}
	}
	//if (stack >= 1 && distance2 <= 150.0f) {
	//	if (g_pad[1]->IsTrigger(enButtonA) && m_kitchenNo == 2 && canGrab == true && m_player[1]->have <= 0) {
	//		//この具材は一回キッチンから帰ってきたか
	//		StackedGuzai[stack - 1]->SetReturnedState(true);
	//		//持たれているか
	//		StackedGuzai[stack - 1]->SetisHadState(true);
	//		//どちらのプレイヤーに持たれているか
	//		StackedGuzai[stack - 1]->SetWhichPlayerGet(2);
	//		//一番上だった具材の番号を９にして何も入っていない状態にする。
	//		m_player[1]->ClearLatestGuzaiNo(stack - 1);
	//		//この具材はキッチンに置かれているか
	//		StackedGuzai[stack - 1]->SetPutState(false);
	//		//現在のスタック数、次のスタック番号を1減らす
	//		stack--;
	//		//プレイヤーは具材を持っている状態にする。
	//		m_player[1]->have = 1;
	//	}
	//}

	//キッチンに5個以上具材があると"コンベアからは"取れないようにする。
	if (stack >= MaxStack && m_kitchenNo == 1) {
		m_player->have = -1;
	}

	BornBurger();

	m_skinModelRender->SetPosition(m_kitchenPos);
	m_skinModelRender->SetScale(m_scale);
}