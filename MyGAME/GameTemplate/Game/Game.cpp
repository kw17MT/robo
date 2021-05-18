#include "stdafx.h"
#include "Game.h"
#include "Level.h"
#include "FixedUI.h"
#include "SpriteRender.h"
#include "Guzai.h"
#include "GuzaiGene.h"
#include "GuzaiOkiba.h"
#include "Kitchen.h"
#include "Counter.h"
#include "Score.h"
#include "FontRender.h"
#include "CLevel2D.h"
#include "Result.h"
#include "SoundSource.h"
#include "PostEffectTest.h"
#include "TrashCan.h"

//#include "ShadowTest.h"
#include "Ground.h"


#include "GameDirector.h"

#include "Player.h"
#include "PlayerGene.h"

#include  "DishGene.h"
#include "DishSpeedManeger.h"

#include "effect/effect.h"
#include "Floor.h"
#include "Arrow.h"

namespace
{
	float  SCALER_COUNTDOWN_FONT_SCALE = 3.0f;
	float MAX_COUNTDOWN_FONT_SCALE = 4.0f;
}

Level level;
//Level2D level2D;

Game::Game()
{	
	ui = NewGO<FixedUI>(1,"ui");

	//リザルトにそれぞれnullptrを入れておく
	//0 : 引き分け(これだけを表示)
	//以下、引き分けでない場合
	//1 : プレイヤー1
	//2 : プレイヤー2

	for (int i = 0; i < 3; i++) {
		m_result[i] = nullptr;
	}

	playerGene = NewGO<PlayerGene>(0, "playerGene");
	dishGene = NewGO<DishGene>(0,"dishGene");
	m_speedManeger = NewGO<DishSpeedManeger>(0, "speedManeger");

	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/BGM/BGM1.wav", false);
	m_bgm->SetVolume(1.0f);
	m_bgm->Play(true);

	////勝敗表示用スプライト
	////表示するポジションを定義
	//win_loseLeft.Set(200.0f, 350.0f);
	//win_loseRight.Set(1080.0f,350.0f);

	////バフアイテムをとった時にアイコンを配置。////////////////////////////////////////////
	//if (player[0]->stateBuff()) {
	//	Buff[0] = NewGO<SpriteRender>(2);
	//	Buff[0]->Init("Assets/Image/icon_speedup.dds", 128, 128);
	//	Vector3 pos = { -500.0f,350.0f,0.0f };
	//	Buff[0]->SetPosition(pos);
	//}

	level.Init("Assets/level/level_new4.tkl", [&](ObjectData& objectData) {
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
			playerGene->SetPlayer01Pos(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"PlayerPos02") == 0) {
			playerGene->SetPlayer02Pos(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"Conveyor") == 0) {
			return false;
		}
		if (wcscmp(objectData.name, L"FloorBlue") == 0) {
			
			return false;
		}
		if (wcscmp(objectData.name, L"FloorRed") == 0) {
			return false;
		}
		
		if (wcscmp(objectData.name, L"TrashLeft") == 0) {
			m_trashCan[0] = NewGO<TrashCan>(0, "trashcan01");
			m_trashCan[0]->SetTrashCanNum(1);
			m_trashCan[0]->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"TrashRight") == 0) {
			m_trashCan[1] = NewGO<TrashCan>(0, "trashcan02");
			m_trashCan[1]->SetTrashCanNum(2);
			m_trashCan[1]->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"floorLeft") == 0) {
			floor_l = NewGO<Floor>(0);
			floor_l->SetRed(true);
			floor_l->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"floorRight") == 0) {
			floor_r = NewGO<Floor>(0);
			floor_r->SetRed(false);
			floor_r->SetPosition(objectData.Pos);
			return true;
		}

		else {
			return true;
		}
		});

	//具材置き場の表示
	guzaiOkiba = NewGO<GuzaiOkiba>(0, "GuzaiOkiba");

	

	////レベル2Dの構築
	//level2D.Init("Assets/level2D/level2D.casl", [&](Level2DObjectData& objectData2D) {return false; });

	NewGO<CLevel2D>(3, "clevel2d");


	//スコアの表示/////////////////////////////////////////////////////////////////////
	m_score = NewGO<Score>(2, "score");

	//ポストエフェクトのテスト用モデル。
	//postTest =  NewGO<PostEffectTest>(5);

	//シャドウのテスト用のモデル
	//shadowTest = NewGO<ShadowTest>(5);

	//これを消す時、LevelのFloorをreturn false にすること。
	//↓シャドウレシーバー。
	//ground = NewGO<Ground>(0);

	//カウントダウン用スプライトの初期化
	m_countSprite = NewGO<SpriteRender>(20);
	Vector2 ctPivot = { 0.5f,0.5f };
	Vector3 ctPosition = { 0.0f,100.0f,0.0f };
	m_countSprite->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_countSprite->SetPivot(ctPivot);
	m_countSprite->SetPosition(ctPosition);


	//カウントダウンを開始するということを設定する。
	GetGameDirector().SetGameScene(enGameCountDown);

	m_font = NewGO<FontRender>(5);
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
		DeleteGO(player[i]);
	}
	for (int i = 0; i < 2; i++) {
		DeleteGO(m_result[i]);
	}
	DeleteGO(guzaiGene);
	DeleteGO(guzaiOkiba);
	DeleteGO(m_score);
	//DeleteGO(m_bgm);
}

bool Game::Start()
{
	return true;
}

void Game::Update()
{
	//カウントダウンする。
	CountDown();

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

void Game::CountDown()
{

	//カウントダウン中じゃなかったら。
	if (!GetGameDirector().GetIsGameCountDown())
	{
		//処理しない。
		return;
	}


	//TODO GameTimeにする。

	//m_timer -= 1.0f / 60.0f;

	//m_timer -= 1.0f / 120.0f;
	m_timer -= 1.0f / 60.0f; //画面のリフレッシュレートが60Hzだったので変更



	if (m_timer <= 0.0f)
	{
		//カウントダウンが終了し、ゲームが開始したことを設定する。
		GetGameDirector().SetGameScene(enGamePlay);
		//DeleteGO(m_font);
		
		//スプライト削除
		if (m_timer < 0.0f) {
			DeleteGO(m_countSprite);
		}

		return;
	}

	/*std::wstring number;
	if (m_timer < 1.0f)
	{
		number = L"Start";
	}
	else {
		number = std::to_wstring(int(m_timer));
	}
	float scale = MAX_COUNTDOWN_FONT_SCALE;
	scale -= SCALER_COUNTDOWN_FONT_SCALE * (m_timer - int(m_timer));

	m_font->SetText(number.c_str());
	m_font->SetColor({ 1.0f,0.0f,0.0f,0.0f });
	m_font->SetPivot({ 0.5f, 0.5f });
	m_font->SetPosition({ -100, 200 });
	m_font->SetScale(scale);*/

	//カウントダウン処理(スプライト)
	//フェードイン、アウト処理
	if (fadeOut == false) {
		alpha += 4.0f / 60.0f;
	}
	else {
		alpha -= 1.0f / 60;
	}

	if (alpha > 1.0f) {
		alpha = 1.0f;
		fadeOut = true;
	}
	else if (alpha < 0.0f) {
		alpha = 0.0f;
	}

	//スプライト変更処理
	if (m_timer <= 4.0f && m_timer > 3.0 && changeCount == 3) {
		m_countSprite->Init("Assets/Image/3.dds", 512, 512);
		alpha = 0.0f;
		fadeOut = false;
		changeCount--;
	}
	else if (m_timer <= 3.0f && m_timer > 2.0 && changeCount == 2) {
		m_countSprite->Init("Assets/Image/2.dds", 512, 512);
		alpha = 0.0f;
		fadeOut = false;
		changeCount--;
	}
	else if (m_timer <= 2.0f && m_timer > 1.0 && changeCount == 1) {
		m_countSprite->Init("Assets/Image/1.dds", 512, 512);
		alpha = 0.0f;
		fadeOut = false;
		changeCount--;
	}
	else if (m_timer <= 1.0f && m_timer > 0.0 && changeCount == 0) {
		m_countSprite->Init("Assets/Image/start.dds", 512, 512);
		alpha = 0.0f;
		fadeOut = false;
		changeCount--;
	}
	
	//開始時のカウントダウンに応じて音を鳴らす。
	if (m_timer < 1.0f && m_soundFlag00 == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/basketball_buzzer1.wav", false);
		se->SetVolume(2.0f);
		se->Play(false);
		m_soundFlag00 = true;
	}
	else if (m_timer < 2.0f && m_soundFlag01 == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Time.wav", false);
		se->SetVolume(2.0f);
		se->Play(false);
		m_soundFlag01 = true;
	}
	else if (m_timer < 3.0f && m_soundFlag02 == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Time.wav", false);
		se->SetVolume(2.0f);
		se->Play(false);
		m_soundFlag02 = true;
	}
	else if (m_timer < 3.9f && m_soundFlag03 == false) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Time.wav", false);
		se->SetVolume(2.0f);
		se->Play(false);
		m_soundFlag03 = true;
	}

	m_countSprite->SetColor(1.0f, 1.0f, 1.0f, alpha);
}