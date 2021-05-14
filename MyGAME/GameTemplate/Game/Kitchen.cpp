#include "stdafx.h"
#include "Kitchen.h"
#include "Burger.h"
#include "Guzai.h"
#include "Counter.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "SoundSource.h"

bool Kitchen::Start()
{
	m_player[0] = FindGO<Player>("player01");
	m_player[1] = FindGO<Player>("player02");

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
		}

		Counter* co01 = FindGO<Counter>("counter01");
		co01->SetStackNum(stack);

		//積んでいる具材数を０で初期化
		stack = 0;
	}
	if (KitchenNo == 2) {
		for (int i = 0;i < stack; i++) {
			DeleteGO(StackedGuzai[i]);
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
	if (KitchenNo == 1) {
		//具材を一つ以上積んでいて、Yボタンを長押し
		if (stack >= 1 && g_pad[0]->IsPress(enButtonY)) {
			Delay--;
			if (m_soundFlag01 == false) {
				//音を鳴らす
				m_soundSource = NewGO<CSoundSource>(0);
				m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
				m_soundSource->SetVolume(2.0f);
				m_soundSource->Play(true);
				m_soundFlag01 = true;
			}
			if (Delay == 0) {
				//キッチンについている具材を全部消去
				Delete();
				//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
				m_player[0]->have = 1;
				bur = NewGO<Burger>(0, "burger01");
				bur->SetBurgerNo(1);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/thi-n.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);
				//音が出ていれば消す。
				if (m_soundFlag01 == true) {
					DeleteGO(m_soundSource);
					m_soundFlag01 = false;
				}
				
				Delay = 60;
			}
		}
		else {
			//音が出ていれば消す。
			if (m_soundFlag01 == true) {
				DeleteGO(m_soundSource);
				m_soundFlag01 = false;
			}
		}
	}
	if (KitchenNo == 2) {
		if (stack >= 1 && g_pad[1]->IsPress(enButtonY)) {
			Delay--;
			if (m_soundFlag02 == false) {
				//音を鳴らす
				m_soundSource = NewGO<CSoundSource>(0);
				m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
				m_soundSource->SetVolume(2.0f);
				m_soundSource->Play(true);
				m_soundFlag02 = true;
			}
			if (Delay == 0) {
				//キッチンについている具材を全部消去
				Delete();
				//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
				m_player[1]->have = 1;
				bur = NewGO<Burger>(0, "burger02");
				bur->SetBurgerNo(2);
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/thi-n.wav", false);
				se->SetVolume(2.0f);
				se->Play(false);
				//音が出ていれば消す。
				if (m_soundFlag02 == true) {
					DeleteGO(m_soundSource);
					m_soundFlag02 = false;
				}
		
				Delay = 60;
			}
		}
		else {
			//音が出ていれば消す。
			if (m_soundFlag02 == true) {
				DeleteGO(m_soundSource);
				m_soundFlag02 = false;
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
		if (g_pad[0]->IsTrigger(enButtonA) && KitchenNo == 1 && canGrab == true && m_player[0]->have == 0) {
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
		if (g_pad[1]->IsTrigger(enButtonA) && KitchenNo == 2 && canGrab == true && m_player[1]->have == 0) {
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

	//キッチンに5個以上具材があると取れないようにする。
	if (stack >= MaxStack && KitchenNo == 1) {
		m_player[0]->have = 1;
	}
	if (stack >= MaxStack && KitchenNo == 2) {
		m_player[1]->have = 1;
	}

	BornBurger();

	m_skinModelRender->SetPosition(m_kitchenPos);
	m_skinModelRender->SetScale(m_scale);
}