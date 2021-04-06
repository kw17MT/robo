#include "stdafx.h"
#include "Game.h"
#include "Level.h"
#include "ModelRender.h"
#include "FixedUI.h"
#include "SpriteRender.h"
#include "Guzai.h"
#include "ObjectGene.h"
#include "Kitchen.h"
#include "Counter.h"
#include "Score.h"
#include "FontRender.h"
#include "CLevel2D.h"
#include "Result.h"
#include "SoundSource.h"

Level level;

Game::Game()
{	
	//文字の描写///////////////////////////////////////////////////////////////////////
	//FixedUIクラスのアップデート内で表示する文字を固定している。
	ui = NewGO<FixedUI>(1,"ui");

	//リザルトにそれぞれnullptrを入れておく
	//0 : 引き分け(これだけを表示)
	//以下、引き分けでない場合
	//1 : プレイヤー1
	//2 : プレイヤー2

	for (int i = 0; i < 3; i++) {
		m_result[i] = nullptr;
	}

	////勝敗表示用スプライト
	////表示するポジションを定義
	//win_loseLeft.Set(200.0f, 350.0f);
	//win_loseRight.Set(1080.0f,350.0f);

	//カウンターの作成/////////////////////////////////////////////////////////////////
	//SetCounterNoでどちら側のカウンターか決定する。
	/*counter01 = NewGO<Counter>(1, "counter01");
	counter01->SetCounterNo(1);
	Vector3 PosCo01 = { 900.0f, 0.0f, 400.0f };
	counter01->SetPosition(PosCo01);

	counter02 = NewGO<Counter>(1, "counter02");
	counter02->SetCounterNo(2);
	Vector3 PosCo02 = { -900.0f, 0.0f, 400.0f };
	counter02->SetPosition(PosCo02);*/

	//キッチンの作成///////////////////////////////////////////////////////////////////
	//カウンターと同様にどちら側か決定。
	/*kitchen01 = NewGO<Kitchen>(0, "kitchen01");
	kitchen01->SetKitchenNo(1);
	Vector3 kiPos01 = { 900.0f, 0.0f, 0.0f };
	kitchen01->SetKitchenPos(kiPos01);

	kitchen02 = NewGO<Kitchen>(0, "kitchen02");
	kitchen02->SetKitchenNo(2);
	Vector3 kiPos02 = { -900.0f, 0.0f, 0.0f };
	kitchen02->SetKitchenPos(kiPos02);*/

	///////////////////////////////////////////////////////////////////////////////
	//スコア
	//m_score = NewGO<Score>(0, "score");
	
	////画像の描写/////////////////////////////////////////////////////////////////////
	////ただし、初期メニューのみ
	/*for (int i = 0; i < 3; i++) {
		menu[i] = NewGO<SpriteRender>(2);
		switch (i) {
		case 0:
			menu[i]->Init("Assets/Image/burger_cheese_new.dds", 128, 256);
			break;
		case 1:
			menu[i]->Init("Assets/Image/burger_tomato.dds", 128, 256);
			break;
		case 2:
			menu[i]->Init("Assets/Image/burger_egg.dds", 128, 256);
			break;
		}

		const float shiftRight = 150.0f;

		Vector3 pos = { -150.0f,-200.0f,0.0f };
		pos.x += i * shiftRight;
		menu[i]->SetPosition(pos);
	}*/

	//NewGO<CLevel2D>(3);

	//カウンターの作成/////////////////////////////////////////////////////////////////
	//SetCounterNoでどちら側のカウンターか決定する。
	//counter01 = NewGO<Counter>(1, "counter01");
	//counter01->SetCounterNo(1);
	//Vector3 PosCo01 = { 0.0f, 0.0f, 0.0f };
	//counter01->SetPosition(PosCo01);

	//counter02 = NewGO<Counter>(1, "counter02");
	//counter02->SetCounterNo(2);
	//Vector3 PosCo02 = { -900.0f, 0.0f, 400.0f };
	//counter02->SetPosition(PosCo02);

	////キッチンの作成///////////////////////////////////////////////////////////////////
	////カウンターと同様にどちら側か決定。
	//kitchen01 = NewGO<Kitchen>(0, "kitchen01");
	//kitchen01->SetKitchenNo(1);
	//Vector3 kiPos01 = { 900.0f, 0.0f, 0.0f };
	//kitchen01->SetKitchenPos(kiPos01);

	//kitchen02 = NewGO<Kitchen>(0, "kitchen02");
	//kitchen02->SetKitchenNo(2);
	//Vector3 kiPos02 = { -900.0f, 0.0f, 0.0f };
	//kitchen02->SetKitchenPos(kiPos02);

	/////////////////////////////////////////////////////////////////////////////////
	
	

	////プレイヤーのモデルを作成x2//////////////////////////////////////////////////////////
	/*player[0] = NewGO<ModelRender>(0, "player01");
	player[0]->SetPlayerNo(1);
	player[1] = NewGO<ModelRender>(0, "player02");
	player[1]->SetPlayerNo(2);*/

	//具材とバフを生成器//////////////////////////////////////////////////////////////////
	/*generator01 = NewGO<ObjectGene>(1, "gene01");
	generator01->SetGeneNo(1);
	Vector3 PosGene01 = { 500.0f,100.0f,-500.0f };
	generator01->SetPosition(PosGene01);

	generator02 = NewGO<ObjectGene>(1, "gene02");
	generator02->SetGeneNo(2);
	Vector3 PosGene02 = { -300.0f,100.0f,-500.0f };
	generator02->SetPosition(PosGene02);*/

	////バフアイテムをとった時にアイコンを配置。////////////////////////////////////////////
	//if (player[0]->stateBuff()) {
	//	Buff[0] = NewGO<SpriteRender>(2);
	//	Buff[0]->Init("Assets/Image/icon_speedup.dds", 128, 128);
	//	Vector3 pos = { -500.0f,350.0f,0.0f };
	//	Buff[0]->SetPosition(pos);
	//}

	level.Init("Assets/level/level_mirror.tkl", [&](ObjectData& objectData) {
		if (wcscmp(objectData.name, L"CounterPos01") == 0) {
			counter01 = NewGO<Counter>(0, "counter01");
			counter01->SetCounterNo(1);
			counter01->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"CounterPos02") == 0) {
			counter02 = NewGO<Counter>(0, "counter02");
			counter02->SetCounterNo(2);
			counter02->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"KitchenPos01") == 0) {
			kitchen01 = NewGO<Kitchen>(0, "kitchen01");
			kitchen01->SetKitchenNo(1);
			kitchen01->SetKitchenPos(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"KitchenPos02") == 0) {
			kitchen02 = NewGO<Kitchen>(0, "kitchen02");
			kitchen02->SetKitchenNo(2);
			kitchen02->SetKitchenPos(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"PlayerPos01") == 0) {
			player[0] = NewGO<ModelRender>(0, "player01");
			player[0]->SetPlayerNo(1);
			player[0]->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"PlayerPos02") == 0) {
			player[1] = NewGO<ModelRender>(0, "player02");
			player[1]->SetPlayerNo(2);
			player[1]->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"GeneratorPos01") == 0) {
			generator01 = NewGO<ObjectGene>(0, "gene01");
			generator01->SetGeneNo(1);
			generator01->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"GeneratorPos02") == 0) {
			generator02 = NewGO<ObjectGene>(0, "gene02");
			generator02->SetGeneNo(2);
			generator02->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"Conveyor") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"Floor") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint011") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint012") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint013") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint014") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint021") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint022") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint023") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"WayPoint024") == 0) {
			return false;
		}
		else {
			return false;
		}
		});

	//文字の描写///////////////////////////////////////////////////////////////////////
	//FixedUIクラスのアップデート内で表示する文字を固定している。
	//ui = NewGO<FixedUI>(2);

	NewGO<CLevel2D>(3);

	//スコアの表示/////////////////////////////////////////////////////////////////////
	m_score = NewGO<Score>(2, "score");

	
}

Game::~Game()
{
	DeleteGO(ui);
	DeleteGO(counter01);
	DeleteGO(counter02);
	DeleteGO(kitchen01);
	DeleteGO(kitchen02);
	for (int i = 0; i < 3;i++) {
		DeleteGO(menu[i]);
	}
	for (int i = 0; i < 2;i++) {
		DeleteGO(Buff[i]);
	}
	for (int i = 0; i < 2;i++) {
		DeleteGO(player[i]);
	}
	DeleteGO(generator01);
	DeleteGO(generator02);
	DeleteGO(m_score);
}

bool Game::Start()
{
	return true;
}

void Game::Update()
{
	level.Draw();

	//タイムアップ時に行う処理
	//結果の表示
	if (ui->GetIsTimeUp() == true && GetTimeUp() == false) {
		
		//引き分けのとき(ResultP1 = 0,ResultP2 = 0のとき)、1枚だけ表示
		if (m_score->ResultP1 == m_score->ResultP2) {
			m_result[0] = NewGO<Result>(1, "result");
			m_result[0]->SetSprite(0); //0 : 引き分け
			m_result[0]->SetSpritePos(0); //中央くらいの位置
		}

		//勝敗が決まっているとき(enumの返す整数値が異なるとき),2枚を表示
		else if (m_score->ResultP1 != m_score->ResultP2) {
			//プレイヤー1
			if (m_score->ResultP1 == 1) {
				m_result[1] = NewGO<Result>(1, "result");
				m_result[1]->SetSprite(1); //1 : 勝利
				m_result[1]->SetSpritePos(1); //1 : 右側
			}
			if (m_score->ResultP1 == 2) {
				m_result[1] = NewGO<Result>(1, "result");
				m_result[1]->SetSprite(2); //2 : 敗北
				m_result[1]->SetSpritePos(1); //1 : 右側
			}

			//プレイヤー2
			if (m_score->ResultP2 == 1) {
				m_result[2] = NewGO<Result>(1, "result");
				m_result[2]->SetSprite(1); //1 : 勝利
				m_result[2]->SetSpritePos(2); //2 :左側
			}
			if (m_score->ResultP2 == 2) {
				m_result[2] = NewGO<Result>(1, "result");
				m_result[2]->SetSprite(2); //2 : 敗北
				m_result[2]->SetSpritePos(2); //2 :左側
			}

		}
		//game内のタイムアップフラグを立てる
		SetTimeUp();
	}
	
}