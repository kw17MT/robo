#include "stdafx.h"
#include "Game.h"
#include "Title.h"
#include "Level.h"
#include "FixedUI.h"
#include "SpriteRender.h"
#include "Guzai.h"
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
#include "DishGene.h"
#include "effect/effect.h"
#include "Floor.h"
#include "Arrow.h"
#include "CycleDirection.h"
#include "CountDown.h"
#include "DishManager.h"
#include <ctime>
#include <cstdlib>
#include <random>
#include "ShadowParam.h"
namespace
{
	const Vector2 COUNTDOWN_PIVOT = { 0.5f,0.5f };
	const Vector3 COUNTDOWN_POS = { 0.0f,100.0f,0.0f };
	const Vector4 COUNTDOWN_START_COLOR = { 1.0f,1.0f,1.0f,0.0f };

	float SCALER_COUNTDOWN_FONT_SCALE = 3.0f;
	float MAX_COUNTDOWN_FONT_SCALE = 4.0f;

	const int RESULT_SPRITE_NUM = 3;
	const int TRASHCAN_NUM = 2;
	const int DIRECTION_SPRITE_NUM = 3;
	const int SET_NUMBER_FOR_PLAYER_ONE = 0;
	const int SET_NUMBER_FOR_PLAYER_TWO = 1;
	const int DIRECTION_SPRITE_LEFT = 0;
	const int DIRECTION_SPRITE_RIGHT = 1;
	const int DIRECTION_SPRITE_UP = 2;
	const int TRASHCAN_LEFT = 0;
	const int TRASHCAN_RIGHT = 1;
	const int RESULT_DRAW = 0;
	const int RESULT_WIN = 1;
	const int RESULT_LOSE = 2;
	const int RESULT_FOR_PLAYER_ONE = 1;
	const int RESULT_FOR_PLAYER_TWO = 2;
	const int COUNTDOWN_SPRITE_WIDTH = 512;
	const int COUNTDOWN_SPRITE_HEIGHT = 512;
	const int COUNTDOWN_PHASE_ONE = 1;
	const int COUNTDOWN_PHASE_TWO = 2;
	const int COUNTDOWN_PHASE_THREE = 3;
	const int COUNTDOWN_PHASE_END = 4;
	const int AJUST_TIMER_SPEED = 2;
	const int SAME_NAME = 0;
	const int PLAYER_ONE_CONTROLLER = 0;

	const float COUNTDOWN_PHASE1_START = 4.0f;
	const float COUNTDOWN_PHASE1_END = 3.0f;
	const float COUNTDOWN_PHASE2_START = 3.0f;
	const float COUNTDOWN_PHASE2_END = 2.0f;
	const float COUNTDOWN_PHASE3_START = 2.0f;
	const float COUNTDOWN_PHASE3_END = 1.0f;
	const float COUNTDOWN_PHASE_FINAL_START = 1.0f;
	const float COUNTDOWN_PHASE_FINAL_END = 0.0f;
	const float COUNTDOWN_SHRINK_SPEED = 2.0f / 60.0f;
	const float COUNTDOWN_START_SCALE = 2.0f;
	const float SE_VOLUME = 1.0f;
}

Game::~Game()
{
	if (m_countDown != nullptr) {
		DeleteGO(m_countDown);
	}

	DeleteGO(floor_r);
	DeleteGO(floor_l);
	
	for (int i = 0; i < RESULT_SPRITE_NUM; i++) {
		if (m_result[i] != nullptr) {
			DeleteGO(m_result[i]);
		}
	}

	DeleteGO(ui);
	DeleteGO(playerGene);
	DeleteGO(dishGene);
	//DeleteGO(guzaiGene);
	DeleteGO(guzaiOkiba);
	DeleteGO(m_score);
	DeleteGO(m_bgm);

	for (int i = 0; i < TRASHCAN_NUM; i++) {
		DeleteGO(m_trashCan[i]);
	}

	for (int i = 0; i < DIRECTION_SPRITE_NUM; i++) {
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
	//2Dのレベルの表示（メニュー3つとMISSの四角6個分）
	m_level2D = NewGO<CLevel2D>(3, "clevel2d");
	//3Dのレベルの表示（コンベアのみ、あとは名前でNewGOしている）
	m_level = NewGO<Level>(0, "level");
	//プレイヤーの生成器を作成
	playerGene = NewGO<PlayerGene>(0, "playerGene");
	//皿の生成器を作成
	dishGene = NewGO<DishGene>(0, "dishGene");
	//レベルを利用したオブジェクトの配置
	m_level->Init("Assets/level/level_new4.tkl", [&](ObjectData& objectData) {
		//カウンターのオブジェクト（左）を出す
		if (wcscmp(objectData.name, L"CounterPos01") == SAME_NAME) {
			counter00 = NewGO<Counter>(0, "counter00");
			counter00->SetCounterNo(SET_NUMBER_FOR_PLAYER_ONE);
			counter00->SetPosition(objectData.Pos);
			return true;
		}
		//カウンターのオブジェクト（右）を出す
		if (wcscmp(objectData.name, L"CounterPos02") == SAME_NAME) {
			counter01 = NewGO<Counter>(0, "counter01");
			counter01->SetCounterNo(SET_NUMBER_FOR_PLAYER_TWO);
			counter01->SetPosition(objectData.Pos);
			return true;
		}
		//キッチン（左）を出す
		if (wcscmp(objectData.name, L"KitchenPos01") == SAME_NAME) {
			kitchen00 = NewGO<Kitchen>(0, "kitchen00");
			kitchen00->SetKitchenNo(SET_NUMBER_FOR_PLAYER_ONE);
			kitchen00->SetKitchenPos(objectData.Pos);
			return true;
		}
		//キッチン（右）を出す
		if (wcscmp(objectData.name, L"KitchenPos02") == SAME_NAME) {
			kitchen01 = NewGO<Kitchen>(0, "kitchen01");
			kitchen01->SetKitchenNo(SET_NUMBER_FOR_PLAYER_TWO);
			kitchen01->SetKitchenPos(objectData.Pos);
			return true;
		}
		//プレイヤー1を出す位置座標を取得する。
		if (wcscmp(objectData.name, L"PlayerPos01") == SAME_NAME) {
			playerGene->SetPlayer01Pos(objectData.Pos);
			return true;
		}
		//プレイヤー2を出す位置座標を取得する
		if (wcscmp(objectData.name, L"PlayerPos02") == SAME_NAME) {
			playerGene->SetPlayer02Pos(objectData.Pos);
			return true;
		}
		//コンベアのモデルを出す。
		if (wcscmp(objectData.name, L"Conveyor") == SAME_NAME) {
			return false;
		}
		//ゴミ箱（左）を出す。
		if (wcscmp(objectData.name, L"TrashLeft") == SAME_NAME) {
			m_trashCan[TRASHCAN_LEFT] = NewGO<TrashCan>(0, "trashcan00");
			m_trashCan[TRASHCAN_LEFT]->SetTrashCanNum(SET_NUMBER_FOR_PLAYER_ONE);
			m_trashCan[TRASHCAN_LEFT]->SetPosition(objectData.Pos);
			return true;
		}
		//ゴミ箱（右）を出す。
		if (wcscmp(objectData.name, L"TrashRight") == SAME_NAME) {
			m_trashCan[TRASHCAN_RIGHT] = NewGO<TrashCan>(0, "trashcan01");
			m_trashCan[TRASHCAN_RIGHT]->SetTrashCanNum(SET_NUMBER_FOR_PLAYER_TWO);
			m_trashCan[TRASHCAN_RIGHT]->SetPosition(objectData.Pos);
			return true;
		}
		//床（左）を出す。
		if (wcscmp(objectData.name, L"floorLeft") == SAME_NAME) {
			floor_l = NewGO<Floor>(0);
			floor_l->SetRed(true);
			floor_l->SetPosition(objectData.Pos);
			return true;
		}
		//床（右）を出す。
		if (wcscmp(objectData.name, L"floorRight") == SAME_NAME) {
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
	//具材の生成、補充を管理するもの
	//UI全般を出すもの
	ui = NewGO<FixedUI>(1, "ui");
	//スコアの表示を行うもの
	m_score = NewGO<Score>(2, "score");
	//カウントダウン用スプライトの初期化
	m_countDown = NewGO<CountDown>(1, "countdown");

	//コンベア回転方向の画像を3枚出す。
	m_directionSprite[DIRECTION_SPRITE_LEFT] = NewGO<CycleDirection>(0, "dirsp1");
	m_directionSprite[DIRECTION_SPRITE_LEFT]->SetDirection(Reverse);				//右回転
	m_directionSprite[DIRECTION_SPRITE_LEFT]->ChangeSpriteReverse();
	m_directionSprite[DIRECTION_SPRITE_LEFT]->SetSide(Left);						//左
	m_directionSprite[DIRECTION_SPRITE_RIGHT] = NewGO<CycleDirection>(0, "dirsp2");
	m_directionSprite[DIRECTION_SPRITE_RIGHT]->SetDirection(Forward);				//左回転
	m_directionSprite[DIRECTION_SPRITE_RIGHT]->ChangeSpriteForward();
	m_directionSprite[DIRECTION_SPRITE_RIGHT]->SetSide(Right);						//右
	m_directionSprite[DIRECTION_SPRITE_UP] = NewGO<CycleDirection>(0, "dirspfixed");
	m_directionSprite[DIRECTION_SPRITE_UP]->SetDirection(FixedForward);				//固定表示
	m_directionSprite[DIRECTION_SPRITE_UP]->SetSide(Up);							//画面上部

	//BGMをならす。
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/BGM/BGM1.wav", false);
	m_bgm->SetVolume(SE_VOLUME);
	m_bgm->Play(true);

	//影を作る光の向きとかのパラメータを初期化
	ShadowParam::GetInstance().SetLightCameraParam({ 0.0f,500.0f,0.0f });
	
	return true;
}

void DishSpeedUp()
{
	//皿のスピードアップまでの時間が0以下であれば
	if (DishManager::GetInstance().GetTimeTillSpeedUp() <= 0) {
		//ランダムな数を取得する。
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<int> rand(9, 15);
		////10~20までの整数を取得する。
		float TimeTillSpeedUp = rand(mt);
		//スピードアップまでの時間を設定してやる。
		DishManager::GetInstance().SetTimeTillSpeedUp(TimeTillSpeedUp);
		float TimeForSpeedUp = TimeTillSpeedUp / 3.0f;
		//スピードアップまでの時間を設定してやる。
		DishManager::GetInstance().SetActiveTimeForSpeedUp(TimeForSpeedUp);
	}
	//スピードアップまでの時間が設定されているor残っているならば
	else {
		//1フレームにかかる時間を取得
		float gameTime = GameTime().GetFrameDeltaTime();
		//1秒に1へらしていく。
		DishManager::GetInstance().DecreaseTimeTillSpeedUp(gameTime);
		//スピードアップまでの時間が０になったら
		if (DishManager::GetInstance().GetTimeTillSpeedUp() <= 0.0f) {
			//現在のスピードアップの状態を逆にする。
			DishManager::GetInstance().SwitchSpeedUpState();
		}
	}
	//現在スピードアップ中ならば
	if (DishManager::GetInstance().GetSpeedUpState() == true) {
		float gameTime = GameTime().GetFrameDeltaTime();
		//ゲームタイムを用いてスピードアップの有効時間を減らしていく。
		DishManager::GetInstance().DecreaseActiveTimeForSpeedUp(gameTime);
		//スピードアップの有効時間がなくなったら
		if (DishManager::GetInstance().GetActiveTimeForSpeedUp() <= 0.0f) {
			//現在のスピードアップの状態を逆にする。（普通の速度にする）
			DishManager::GetInstance().SwitchSpeedUpState();
		}
	}
}

void Game::DoWhenTimeUp()
{
	//結果の表示
	if (ui->GetIsTimeUp() == true && GetTimeUp() == false) {

		//引き分けのとき(ResultP1 = 0,ResultP2 = 0のとき)、1枚だけ表示
		if (m_score->ResultP1 == m_score->ResultP2) {
			m_result[RESULT_DRAW] = NewGO<Result>(1, "result");
			m_result[RESULT_DRAW]->SetSprite(RESULT_DRAW);									//0 : 引き分け
			m_result[RESULT_DRAW]->SetSpritePos(RESULT_DRAW);								//中央くらいの位置
		}

		//勝敗が決まっているとき(enumの返す整数値が異なるとき),2枚を表示
		else if (m_score->ResultP1 != m_score->ResultP2) {
			//プレイヤー1
			if (m_score->ResultP1 == RESULT_WIN) {
				m_result[RESULT_FOR_PLAYER_ONE] = NewGO<Result>(1, "result");
				m_result[RESULT_FOR_PLAYER_ONE]->SetSprite(RESULT_WIN);						//1 : 勝利
				m_result[RESULT_FOR_PLAYER_ONE]->SetSpritePos(RESULT_FOR_PLAYER_ONE);		//1 : 右側
			}
			if (m_score->ResultP1 == RESULT_LOSE) {
				m_result[RESULT_FOR_PLAYER_ONE] = NewGO<Result>(1, "result");
				m_result[RESULT_FOR_PLAYER_ONE]->SetSprite(RESULT_LOSE);					//2 : 敗北
				m_result[RESULT_FOR_PLAYER_ONE]->SetSpritePos(RESULT_FOR_PLAYER_ONE);		//1 : 右側
			}

			//プレイヤー2
			if (m_score->ResultP2 == RESULT_WIN) {
				m_result[RESULT_FOR_PLAYER_TWO] = NewGO<Result>(1, "result");
				m_result[RESULT_FOR_PLAYER_TWO]->SetSprite(RESULT_WIN);						//1 : 勝利
				m_result[RESULT_FOR_PLAYER_TWO]->SetSpritePos(RESULT_FOR_PLAYER_TWO);		//2 :左側
			}
			if (m_score->ResultP2 == RESULT_LOSE) {
				m_result[RESULT_FOR_PLAYER_TWO] = NewGO<Result>(1, "result");
				m_result[RESULT_FOR_PLAYER_TWO]->SetSprite(RESULT_LOSE);					//2 : 敗北
				m_result[RESULT_FOR_PLAYER_TWO]->SetSpritePos(RESULT_FOR_PLAYER_TWO);		//2 :左側
			}

		}
		//game内のタイムアップフラグを立て、ゲームシーンをリザルトに移行する
		SetTimeUp();

		GetGameDirector().SetGameScene(enResult);
		//ゲーム終了を通知
	}
}

void Game::Update()
{
	//レベルの描画
	m_level->Draw();

	//タイムアップ時に行う処理
	DoWhenTimeUp();

	//たまに皿の速度が上がる処理。ランダム性を持たすためにUpdate関数に書かなくてはならない
	DishSpeedUp();

	//リザルト中にプレイヤー1がAボタンを押すとタイトルに移行
	if (GetGameDirector().GetGameScene() == enResult) {
		if (g_pad[PLAYER_ONE_CONTROLLER]->IsTrigger(enButtonA)) {
			GetGameDirector().SetGameScene(enGameEnd);
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
}