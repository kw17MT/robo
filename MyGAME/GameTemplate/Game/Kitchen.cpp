#include "stdafx.h"
#include "Kitchen.h"
//#include "ModelRender.h"
#include "Burger.h"
#include "Guzai.h"
#include "Counter.h"

#include "Player.h"
#include "SkinModelRender.h"

Kitchen::Kitchen()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/object/kitchen.tkm", nullptr, enModelUpAxisZ, m_position);
	m_scale = { 0.3f,0.3f,0.3f };
	m_skinModelRender->SetScale(m_scale);

	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

}

void Kitchen::ChangeModel(const char* filePath)
{
	m_skinModelRender->Init(filePath, nullptr, enModelUpAxisZ, m_position);
}

//具材の位置をキッチンの上に設定する
void Kitchen::Stack(int num)
{
	if (KitchenNo == 1) {
		Player* pl01 = FindGO<Player>("player01");
		
		//下のif文でスタックが完了したかの状態を変更後、次フレームで実行できるように上に置いている。
		if (isCompletedStack == true) {
			//生成した具材のモデルを、プレイヤーに保存された具材配列の番号を元に決定する
			StackedGuzai[stack - 1]->ChangeGuzai(pl01->GuzaiNo[stack - 1]);
			//モデルをキッチン用のモデルに差し替える用
			StackedGuzai[stack - 1]->ChangeModel(pl01->GuzaiNo[stack - 1]);
			//(現在の段数) * 高さ の場所に具材を生成する
			m_position.y = stack * 60.0f;
			StackedGuzai[nextStackNum - 1]->SetPosition(m_position);
			//具材が置かれるのを待つ状態にする
			isCompletedStack = false;
		}

		if (nextStackNum < stack) {
			StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
			//ここで、キッチンに「置かれた」ことを記録し、キッチン上から動かないようにする。
			StackedGuzai[nextStackNum]->put = 1;

			isCompletedStack = true;
			nextStackNum++;
		}
	}

	if (KitchenNo == 2) {
		Player* pl02 = FindGO<Player>("player02");

		if (isCompletedStack == true) {
			StackedGuzai[stack - 1]->ChangeGuzai(pl02->GuzaiNo[stack - 1]);
			//モデルをキッチン用のモデルに差し替える用
			StackedGuzai[stack - 1]->ChangeModel(pl02->GuzaiNo[stack - 1]);
			m_position.y = stack * 60.0f;
			StackedGuzai[nextStackNum - 1]->SetPosition(m_position);

			isCompletedStack = false;
		}

		if (nextStackNum < stack) {
			StackedGuzai[nextStackNum] = NewGO<Guzai>(0);
			StackedGuzai[nextStackNum]->put = 1;

			isCompletedStack = true;
			nextStackNum++;
		}
	}
}

//積んでいる具材を全部消去する。
//スタックに使った変数を全部初期化。
void Kitchen::Delete()
{
	if (KitchenNo == 1) {
		for (int i = 0;i < nextStackNum; i++) {
			DeleteGO(StackedGuzai[i]);
			//次に積む具材のY座標をゼロにする。
			m_position.y = 0;
		}

		Counter* co01 = FindGO<Counter>("counter01");
		co01->SetStackNum(stack);

		stack = 0;
		nextStackNum = 0;
		DeleteTimer = 0;
		/*ModelRender*/Player* pl01 = FindGO<Player/*ModelRender*/>("player01");
		pl01->have = 0;
	}
	if (KitchenNo == 2) {
		for (int i = 0;i < nextStackNum; i++) {
			DeleteGO(StackedGuzai[i]);
		}
		Counter* co02 = FindGO<Counter>("counter02");
		co02->SetStackNum(stack);

		stack = 0;
		nextStackNum = 0;
		DeleteTimer = 0;
		/*ModelRender*/Player* pl02 = FindGO<Player/*ModelRender*/>("player02");
		pl02->have = 0;
	}
}

// Delayは必要
// ないとエラー
void Kitchen::BornBurger()
{
	if (KitchenNo == 1) {
		if (nextStackNum >= 1 && g_pad[0]->IsPress(enButtonY)) {
			Delay--;
			if (Delay == 0) {
				Player* pl01 = FindGO<Player>("player01");

				//ここで具材が持っている種類No.をプレイヤーが持っているNo.格納用配列にいれていく。
				//ここでハンバーガーの具材を記録してる。
				for (int i = 0;i < nextStackNum; i++) {
					pl01->GuzaiNo[i] = StackedGuzai[i]->GetTypeNo();
				}
				//キッチンについている具材を全部消去
				Delete();
				//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
				pl01->have = 1;
				bur = NewGO<Burger>(0, "burger01");
				bur->SetBurgerNo(1);
				//間違ったバーガー作ったら、Xボタン長押しで削除するやつ
				bur->burgerExist = 1;

				Delay = 60;
			}
		}
	}
	if (KitchenNo == 2) {
		if (nextStackNum >= 1 && g_pad[1]->IsPress(enButtonY)) {
			Delay--;
			if (Delay == 0) {
				/*ModelRender*/Player* pl02 = FindGO<Player/*ModelRender*/>("player02");

				//ここで具材の種類No.をプレイヤーが持っているNo.格納用配列にいれていく。
				for (int i = 0;i < nextStackNum; i++) {
					pl02->GuzaiNo[i] = StackedGuzai[i]->GetTypeNo();
				}
				//キッチンについている具材を全部消去
				Delete();
				//ここを１にしていることで、ハンバーガーができているとき具材をとれないようにしておく。
				pl02->have = 1;
				bur = NewGO<Burger>(0, "burger02");
				bur->SetBurgerNo(2);
				bur->burgerExist = 1;

				Delay = 60;
			}
		}
	}
}

void Kitchen::ClearNo()
{
	if (KitchenNo == 1) {
		Player* pl01 = FindGO<Player>("player01");

		for (int i = 0;i < nextStackNum; i++) {
			pl01->GuzaiNo[i] = 9;
		}
	}
	if (KitchenNo == 2) {
		Player* pl02 = FindGO<Player>("player02");

		for (int i = 0;i < nextStackNum; i++) {
			pl02->GuzaiNo[i] = 9;
		}
	}
}

void Kitchen::Update()
{
	Stack(stack);

	//Xボタン長押しでプレイヤーに格納している積み上げた具材の記録を初期化、キッチン上に置かれている具材の削除。
	if (g_pad[0]->IsPress(enButtonX) && KitchenNo == 1) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
			ClearNo();
			Delete();
		}
	}
	if (g_pad[1]->IsPress(enButtonX) && KitchenNo == 2) {
		DeleteTimer++;
		if (DeleteTimer > 50) {
			ClearNo();
			Delete();
		}
	}

	//キッチンに5個以上具材があると取れないようにする。
	//Guzaicppでキッチンの場所が上がっていくため機能していない。

	if (nextStackNum >= MaxStack && KitchenNo == 1) {
		Player* pl01 = FindGO<Player>("player01");
		pl01->have = 1;
	}
	if (nextStackNum >= MaxStack && KitchenNo == 2) {
		Player* pl02 = FindGO<Player>("player02");
		pl02->have = 1;
	}

	BornBurger();

	m_skinModelRender->SetPosition(m_kitchenPos);
	m_skinModelRender->SetScale(m_scale);
}