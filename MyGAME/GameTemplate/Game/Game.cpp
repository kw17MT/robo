#include "stdafx.h"
#include "Game.h"
#include "Title.h"
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
#include "TrashCan.h"
#include "GameDirector.h"
#include "Player.h"
#include "PlayerGene.h"
#include  "DishGene.h"
#include "DishSpeedManeger.h"
#include "effect/effect.h"
#include "Floor.h"
#include "Arrow.h"
#include "CycleDirection.h"

namespace
{
	float  SCALER_COUNTDOWN_FONT_SCALE = 3.0f;
	float MAX_COUNTDOWN_FONT_SCALE = 4.0f;
}

Game::Game()
{	
	
}

Game::~Game()
{
	if (m_countSprite != nullptr) {
		DeleteGO(m_countSprite);
	}

	DeleteGO(floor_r);
	DeleteGO(floor_l);
	
	for (int i = 0; i < 3; i++) {
		if (m_result[i] != nullptr) {
			DeleteGO(m_result[i]);
		}
	}

	DeleteGO(ui);
	DeleteGO(playerGene);
	DeleteGO(dishGene);
	DeleteGO(m_speedManeger);
	DeleteGO(guzaiGene);
	DeleteGO(guzaiOkiba);
	DeleteGO(m_score);
	DeleteGO(m_bgm);

	for (int i = 0; i < 2; i++) {
		DeleteGO(m_trashCan[i]);
	}

	for (int i = 0; i < 3; i++) {
		DeleteGO(m_directionSprite[i]);
	}

	DeleteGO(counter00);
	DeleteGO(counter01);
	DeleteGO(kitchen00);
	DeleteGO(kitchen01);
	DeleteGO(m_level2D);
	DeleteGO(m_level);

}

bool Game::Start()
{
	m_level2D = NewGO<CLevel2D>(3, "clevel2d");
	m_level = NewGO<Level>(0, "level");

	//リザルトにそれぞれnullptrを入れておく
	//0 : 引き分け(これだけを表示)
	//以下、引き分けでない場合
	//1 : プレイヤー1
	//2 : プレイヤー2
	for (int i = 0; i < 3; i++) {
		m_result[i] = nullptr;
	}

	playerGene = NewGO<PlayerGene>(0, "playerGene");
	dishGene = NewGO<DishGene>(0, "dishGene");
	m_speedManeger = NewGO<DishSpeedManeger>(0, "speedManeger");

	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/BGM/BGM1.wav", false);
	m_bgm->SetVolume(1.0f);
	m_bgm->Play(true);

	m_level->Init("Assets/level/level_new4.tkl", [&](ObjectData& objectData) {
		if (wcscmp(objectData.name, L"CounterPos01") == 0) {
			counter00 = NewGO<Counter>(0, "counter00");
			counter00->SetCounterNo(0);
			counter00->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"CounterPos02") == 0) {
			counter01 = NewGO<Counter>(0, "counter01");
			counter01->SetCounterNo(1);
			counter01->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"KitchenPos01") == 0) {
			kitchen00 = NewGO<Kitchen>(0, "kitchen00");
			kitchen00->SetKitchenNo(0);
			kitchen00->SetKitchenPos(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"KitchenPos02") == 0) {
			kitchen01 = NewGO<Kitchen>(0, "kitchen01");
			kitchen01->SetKitchenNo(1);
			kitchen01->SetKitchenPos(objectData.Pos);

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
			m_trashCan[0] = NewGO<TrashCan>(0, "trashcan00");
			m_trashCan[0]->SetTrashCanNum(1);
			m_trashCan[0]->SetPosition(objectData.Pos);
			return true;
		}
		if (wcscmp(objectData.name, L"TrashRight") == 0) {
			m_trashCan[1] = NewGO<TrashCan>(0, "trashcan01");
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
	guzaiGene = NewGO<GuzaiGene>(0, "GuzaiGene");

	////レベル2Dの構築
	//level2D.Init("Assets/level2D/level2D.casl", [&](Level2DObjectData& objectData2D) {return false; });

	//UI
	ui = NewGO<FixedUI>(1, "ui");

	//スコアの表示/////////////////////////////////////////////////////////////////////
	m_score = NewGO<Score>(2, "score");

	//カウントダウン用スプライトの初期化
	m_countSprite = NewGO<SpriteRender>(20,"count");					//カウントダウン用　終了時にDeleteされる
	Vector2 ctPivot = { 0.5f,0.5f };
	Vector3 ctPosition = { 0.0f,100.0f,0.0f };
	m_countSprite->SetColor(1.0f, 1.0f, 1.0f, 0.0f);
	m_countSprite->SetPivot(ctPivot);
	m_countSprite->SetPosition(ctPosition);

	//コンベア回転方向
	m_directionSprite[0] = NewGO<CycleDirection>(0, "dirsp1");
	m_directionSprite[0]->SetDirection(Forward);				//右回転
	m_directionSprite[0]->SetSide(Left);						//左
	m_directionSprite[1] = NewGO<CycleDirection>(0, "dirsp2");
	m_directionSprite[1]->SetDirection(Reverse);				//左回転
	m_directionSprite[1]->SetSide(Right);						//右
	m_directionSprite[2] = NewGO<CycleDirection>(0, "dirspfixed");
	m_directionSprite[2]->SetDirection(FixedForward);			//固定表示
	m_directionSprite[2]->SetSide(Up);							//画面上部

	//カウントダウンを開始するということを設定する。
	GetGameDirector().SetGameScene(enGameCountDown);

	return true;
}

void Game::Update()
{
	//OutputDebugStringA("beforeUpdate\n");
	//カウントダウンする。
	//OutputDebugStringA("bforCountDown\n");
	CountDown();
	//OutputDebugStringA("aftCountDown\n");

	//OutputDebugStringA("bforLevelDraw\n");
	m_level->Draw();
	//OutputDebugStringA("aftLevelDraw\n");

	//タイムアップ時に行う処理
	//結果の表示
	//OutputDebugStringA("bforShowResult\n");
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
		//OutputDebugStringA("aftShowResult\n");
		//game内のタイムアップフラグを立て、ゲームシーンをリザルトに移行する
		//OutputDebugStringA("bforSetTimeUp\n");
		SetTimeUp();
		//OutputDebugStringA("aftSetTimeUp\n");

		//OutputDebugStringA("bforSetGameScene\n");
		GetGameDirector().SetGameScene(enResult);
		//OutputDebugStringA("aftSetGameScene\n");
		////ゲーム終了を通知
		//GetGameDirector().SetGameScene(enGameEnd);

	}

	//リザルト中にボタンを押すとタイトルに移行
	if (GetGameDirector().GetGameScene() == enResult) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			GetGameDirector().SetGameScene(enNonScene);
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}

	/*if (GetGameDirector().GetGameScene() == enGamePlay) {
		if (g_pad[0]->IsTrigger(enButtonRB1)) {
			GetGameDirector().SetGameScene(enPause);
		}
	}

	if (GetGameDirector().GetGameScene() == enPause) {
		if (g_pad[0]->IsTrigger(enButtonRB1)) {
			GetGameDirector().SetGameScene(enGamePlay);
		}
	}*/

	//OutputDebugStringA("afterUpdate\n");
}

void Game::CountDown()
{
	//OutputDebugStringA("bforIsGameCountDown\n");
	//カウントダウン中じゃなかったら。
	if (!GetGameDirector().GetIsGameCountDown())
	{
		//処理しない。
		return;
	}
	//OutputDebugStringA("aftIsGammeCountDown\n");


	//TODO GameTimeにする。

	//m_timer -= 1.0f / 60.0f;

	//m_timer -= 1.0f / 120.0f;
	m_timer -= 1.0f / 60.0f; //画面のリフレッシュレートが60Hzだったので変更


	//OutputDebugStringA("bforSetGameScene\n");
	//スプライトの不透明度が0になったらゲームスタート
	if (m_timer <= 0.0f && alpha <= 0.0f){
		//カウントダウンが終了し、ゲームが開始したことを設定する。
		GetGameDirector().SetGameScene(enGamePlay);
		DeleteGO(m_font);
		
		//スプライト削除
		DeleteGO (m_countSprite);
		m_countSprite = nullptr;

		return;
	}
	//OutputDebugStringA("aftSetGameScene\n");

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

	//スプライト変更処理
	//OutputDebugStringA("bforSpriteChange\n");
	if (m_countSprite == nullptr)
	{

	}
	else if (m_timer <= 4.0f && m_timer > 3.0 && changeCount == 3 && a == 0) {
		//OutputDebugStringA("Image3\n");
		m_countSprite->Init("Assets/Image/3.dds", 512, 512);
		alpha = 0.0f;
		scaleRate = 2.0f;
		fadeOut = false;
		changeCount--;
		//OutputDebugStringA("afterImage3\n");
		a++;
	}
	else if (m_timer <= 3.0f && m_timer > 2.0 && changeCount == 2 && a == 1) {
		//OutputDebugStringA("Image2\n");
		m_countSprite->Init("Assets/Image/2.dds", 512, 512);
		alpha = 0.0f;
		scaleRate = 2.0f;
		fadeOut = false;
		changeCount--;
		//OutputDebugStringA("afterImage2\n");
		a++;
	}
	else if (m_timer <= 2.0f && m_timer > 1.0 && changeCount == 1 && a == 2) {
		DeleteGO(m_countSprite);
		m_countSprite = nullptr;
		m_countSprite = NewGO<SpriteRender>(20, "count");
		Vector2 ctPivot = { 0.5f,0.5f };
		Vector3 ctPosition = { 0.0f,100.0f,0.0f };
		m_countSprite->SetColor(1.0f, 1.0f, 1.0f, 0.0f);

		m_countSprite->SetPivot(ctPivot);
		m_countSprite->SetPosition(ctPosition);
		//OutputDebugStringA("Image1\n");
		m_countSprite->Init("Assets/Image/1.dds", 512, 512);
		//OutputDebugStringA("1\n");
		alpha = 0.0f;
		//OutputDebugStringA("2\n");
		scaleRate = 2.0f;
		//OutputDebugStringA("3\n");
		fadeOut = false;
		//OutputDebugStringA("4\n");
		changeCount--;
		///OutputDebugStringA("afterImage1\n");
		a++;
	}
	else if (m_timer <= 1.0f && m_timer > 0.0 && changeCount == 0 && a == 3) {
		//OutputDebugStringA("ImageStart\n");
		m_countSprite->Init("Assets/Image/start.dds", 512, 512);
		alpha = 0.0f;
		scaleRate = 2.0f;
		fadeOut = false;
		changeCount--;
		//OutputDebugStringA("afterImageStart\n");
		a++;
	}

	//OutputDebugStringA("aftSpriteChange\n");

	//カウントダウン処理(スプライト)
	//フェードイン、アウト処理
	if (fadeOut == false) {
		alpha += 1.5f / 60.0f;
	}
	else {
		if (changeCount == -1) {	//スタートの表示だけ早く消えてもらう
			alpha -= 2.5f / 60.0f;
		}
		else {
			alpha -= 1.5f / 60.0f;
		}
	}

	if (alpha > 1.0f) {
		alpha = 1.0f;
		fadeOut = true;
	}
	else if (alpha < 0.0f) {
		alpha = 0.0f;
	}

	//スケール縮小処理
	scaleRate -= 2.0f / 60.0f;
	if (scaleRate < 1.0f) {
		scaleRate = 1.0f;
	}

	//OutputDebugStringA("bforSound\n");
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
	///OutputDebugStringA("aftSound\n");

	m_ctScale = { scaleRate,scaleRate,1.0f };

	m_countSprite->SetColor(1.0f, 1.0f, 1.0f, alpha);
	m_countSprite->SetScale(m_ctScale);

	//OutputDebugStringA("\n");
}