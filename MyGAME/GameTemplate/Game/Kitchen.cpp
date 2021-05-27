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
	m_player[0] = FindGO<Player>("player01");
	m_player[1] = FindGO<Player>("player02");
	m_guzaiGene = FindGO<GuzaiGene>("GuzaiGene");

	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/object/kitchen.tkm", nullptr, enModelUpAxisZ, m_position);
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_scale = { 0.3f,0.3f,0.3f };
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
	if (KitchenNo == 1) {
		for (int i = 0;i < stack; i++) {
			DeleteGO(StackedGuzai[i]);
			m_guzaiGene->SetGuzaiFlag(StackedGuzai[i]->GetGuziNo(), false);
		}

		Counter* co01 = FindGO<Counter>("counter01");
		co01->SetStackNum(stack);

		//積んでいる具材数を０で初期化
		stack = 0;
	}
	if (KitchenNo == 2) {
		for (int i = 0;i < stack; i++) {
			DeleteGO(StackedGuzai[i]);
			m_guzaiGene->SetGuzaiFlag(StackedGuzai[i]->GetGuziNo(), false);
		}
		Counter* co02 = FindGO<Counter>("counter02");
		co02->SetStackNum(stack);

		stack = 0;
	}
}

// Delayは必要
// ないとエラー
void Kitchen::BornBurger()
{
	Vector3 predistance1 = m_player[0]->GetPosition() - m_position;
	float distance1 = predistance1.Length();
	Vector3 predistance2 = m_player[1]->GetPosition() - m_position;
	float distance2 = predistance2.Length();
	if (KitchenNo == 1) {
		//具材を一つ以上積んでいて、Bボタンを長押し
		if (stack >= 1 && g_pad[0]->IsPress(enButtonB) && m_deleteFlag == false && distance1 < 100.0f) {
			Delay--;
			//プレイヤーが動けないようにする。
			m_player[0]->StopMove02(true);
			m_player[0]->SetMoveSpeed({ 0.0f,0.0f,0.0f });
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
				m_player[0]->have = 1;
				//削除フラグを立てる。
				m_deleteFlag = true;
				Delay = 40;
			}
		}
		else {
			if (m_deleteFlag == false) {
				Delay = 60;
				m_player[0]->StopMove02(false);
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
			m_player[0]->have = 1;
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
				
				bur = NewGO<Burger>(0, "burger01");
				bur->SetBurgerNo(1);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/thi-n.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);
				
				m_player[0]->StopMove02(false);
				m_deleteFlag = false;
				Delay = 60;
			}
		}
	}

	if (KitchenNo == 2) {
		if (stack >= 1 && g_pad[1]->IsPress(enButtonB) && m_deleteFlag == false && distance2 < 100.0f) {
			Delay--;
			m_player[1]->StopMove02(true);
			m_player[1]->SetMoveSpeed({ 0.0f,0.0f,0.0f });
			if (m_soundFlag02 == false) {
				m_meter = NewGO<Meter>(0);
				Vector3 pos = m_position;
				pos.x += 100.0f;
				pos.y += 300.0f;
				pos.z += 100.0f;
				m_meter->SetPosition(pos);
				//音を鳴らす
				m_soundSource = NewGO<CSoundSource>(0);
				m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
				m_soundSource->SetVolume(2.0f);
				m_soundSource->Play(true);
				m_soundFlag02 = true;
			}
			Vector3 scale = m_meter->GetScale();
			scale.x -= 1.4 / 60;
			m_meter->SetScale(scale);
			if (Delay == 0) {
				//音が出ていれば消す。
				if (m_soundFlag02 == true) {
					DeleteGO(m_meter);
					DeleteGO(m_soundSource);
					m_soundFlag02 = false;
				}
				//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
				m_player[1]->have = 1;
				m_deleteFlag = true;
				Delay = 40;
			}
		}
		else {
			if (m_deleteFlag == false) {
				Delay = 60;
				//m_player[1]->StopMove02(false);
			}
			//音が出ていれば消す。
			if (m_soundFlag02 == true) {
				DeleteGO(m_meter);
				DeleteGO(m_soundSource);
				m_soundFlag02 = false;
			}
		}
		if (m_deleteFlag == true) {
			m_player[1]->have = 1;
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
			
				bur = NewGO<Burger>(0, "burger02");
				bur->SetBurgerNo(2);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/thi-n.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);

				m_player[1]->StopMove02(false);
				m_deleteFlag = false;
				Delay = 60;
				
			}
		}
	}
}

void Kitchen::ClearNo()
{
	if (KitchenNo == 1) {
		for (int i = 0;i < stack; i++) {
			m_player[0]->GuzaiNo[i] = 9;
		}
	}
	if (KitchenNo == 2) {
		for (int i = 0;i < stack; i++) {
			m_player[1]->GuzaiNo[i] = 9;
		}
	}
}

void Kitchen::Update()
{
	if (m_player[0] == nullptr) {
		m_player[0] = FindGO<Player>("player01");
	}
	if (m_player[1] == nullptr) {
		m_player[1] = FindGO<Player>("player02");
	}

	Vector3 predistance1 = m_player[0]->GetPosition() - m_position;
	float distance1 = predistance1.Length();
	Vector3 predistance2 = m_player[1]->GetPosition() - m_position;
	float distance2 = predistance2.Length();

	//ここいる
	//取る処理
	//1層以上積まれていたらとれるようにする。
	if (stack >= 1 && distance1 <= 150.0f) {
		if (g_pad[0]->IsTrigger(enButtonA) && KitchenNo == 1 && canGrab == true && m_player[0]->have <= 0) {
			//この具材は一回キッチンから帰ってきたか
			StackedGuzai[stack - 1]->returned = true;
			//持たれているか
			StackedGuzai[stack - 1]->state = 1;
			//どちらのプレイヤーに持たれているか
			StackedGuzai[stack - 1]->SetWhichPlayerGet(1);
			//一番上だった具材の番号を９にして何も入っていない状態にする。
			m_player[0]->ClearLatestGuzaiNo(stack - 1);
			//この具材はキッチンに置かれているか
			StackedGuzai[stack - 1]->put = 0;
			//現在のスタック数、次のスタック番号を1減らす
			stack--;
			//プレイヤーは具材を持っている状態にする。
			m_player[0]->have = 1;
		}
	}
	if (stack >= 1 && distance2 <= 150.0f) {
		if (g_pad[1]->IsTrigger(enButtonA) && KitchenNo == 2 && canGrab == true && m_player[1]->have <= 0) {
			//この具材は一回キッチンから帰ってきたか
			StackedGuzai[stack - 1]->returned = true;
			//持たれているか
			StackedGuzai[stack - 1]->state = 1;
			//どちらのプレイヤーに持たれているか
			StackedGuzai[stack - 1]->SetWhichPlayerGet(2);
			//一番上だった具材の番号を９にして何も入っていない状態にする。
			m_player[1]->ClearLatestGuzaiNo(stack - 1);
			//この具材はキッチンに置かれているか
			StackedGuzai[stack - 1]->put = 0;
			//現在のスタック数、次のスタック番号を1減らす
			stack--;
			//プレイヤーは具材を持っている状態にする。
			m_player[1]->have = 1;
		}
	}

	//キッチンに5個以上具材があると"コンベアからは"取れないようにする。
	if (stack >= MaxStack && KitchenNo == 1) {
		m_player[0]->have = -1;
	}
	if (stack >= MaxStack && KitchenNo == 2) {
		m_player[1]->have = -1;
	}

	BornBurger();

	m_skinModelRender->SetPosition(m_kitchenPos);
	m_skinModelRender->SetScale(m_scale);
}