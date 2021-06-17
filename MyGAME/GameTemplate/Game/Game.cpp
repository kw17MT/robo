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
	if (m_countSprite != nullptr) {
		DeleteGO(m_countSprite);
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
	DeleteGO(m_speedManeger);
	DeleteGO(guzaiGene);
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
	//皿の速度管理をするもの
	m_speedManeger = NewGO<DishSpeedManeger>(0, "speedManeger");
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
	guzaiGene = NewGO<GuzaiGene>(0, "GuzaiGene");
	//UI全般を出すもの
	ui = NewGO<FixedUI>(1, "ui");
	//スコアの表示を行うもの
	m_score = NewGO<Score>(2, "score");
	//カウントダウン用スプライトの初期化
	m_countSprite = NewGO<SpriteRender>(20,"count");					
	m_countSprite->SetColor(COUNTDOWN_START_COLOR);
	m_countSprite->SetPivot(COUNTDOWN_PIVOT);
	m_countSprite->SetPosition(COUNTDOWN_POS);

	//コンベア回転方向の画像を3枚出す。
	m_directionSprite[DIRECTION_SPRITE_LEFT] = NewGO<CycleDirection>(0, "dirsp1");
	m_directionSprite[DIRECTION_SPRITE_LEFT]->SetDirection(Forward);				//右回転
	m_directionSprite[DIRECTION_SPRITE_LEFT]->SetSide(Left);						//左
	m_directionSprite[DIRECTION_SPRITE_RIGHT] = NewGO<CycleDirection>(0, "dirsp2");
	m_directionSprite[DIRECTION_SPRITE_RIGHT]->SetDirection(Reverse);				//左回転
	m_directionSprite[DIRECTION_SPRITE_RIGHT]->SetSide(Right);						//右
	m_directionSprite[DIRECTION_SPRITE_UP] = NewGO<CycleDirection>(0, "dirspfixed");
	m_directionSprite[DIRECTION_SPRITE_UP]->SetDirection(FixedForward);			//固定表示
	m_directionSprite[DIRECTION_SPRITE_UP]->SetSide(Up);							//画面上部

	//BGMをならす。
	m_bgm = NewGO<CSoundSource>(0);
	m_bgm->Init(L"Assets/sound/BGM/BGM1.wav", false);
	m_bgm->SetVolume(SE_VOLUME);
	m_bgm->Play(true);

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

	
	//OutputDebugStringA("bforShowResult\n");
	//タイムアップ時に行う処理
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
		//OutputDebugStringA("aftShowResult\n");
		
		//OutputDebugStringA("bforSetTimeUp\n");
		//game内のタイムアップフラグを立て、ゲームシーンをリザルトに移行する
		SetTimeUp();
		//OutputDebugStringA("aftSetTimeUp\n");
		//OutputDebugStringA("bforSetGameScene\n");
		GetGameDirector().SetGameScene(enResult);
		//OutputDebugStringA("aftSetGameScene\n");
		////ゲーム終了を通知

	}

	//リザルト中にプレイヤー1がAボタンを押すとタイトルに移行
	if (GetGameDirector().GetGameScene() == enResult) {
		if (g_pad[PLAYER_ONE_CONTROLLER]->IsTrigger(enButtonA)) {
			GetGameDirector().SetGameScene(enNonScene);
			NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
	//OutputDebugStringA("afterUpdate\n");
}

void Game::CountDown()
{
	//1フレームにかかる時間を取得（秒）
	float gameTime = GameTime().GetFrameDeltaTime();
	//OutputDebugStringA("bforIsGameCountDown\n");
	//カウントダウン中じゃなかったら。
	if (!GetGameDirector().GetIsGameCountDown())
	{
		//処理しない。
		return;
	}
	//OutputDebugStringA("aftIsGammeCountDown\n");
	//カウントダウンの経過時間をゲーム時間を用いて計測
	m_timer -= gameTime / AJUST_TIMER_SPEED; 

	//OutputDebugStringA("bforSetGameScene\n");
	//スプライトの不透明度が0になったらゲームスタート
	if (m_timer <= COUNTDOWN_PHASE_FINAL_END && alpha <= COUNTDOWN_PHASE_FINAL_END){
		//カウントダウンが終了し、ゲームが開始したことを設定する。
		GetGameDirector().SetGameScene(enGamePlay);
		DeleteGO(m_font);
		
		//スプライト削除
		DeleteGO (m_countSprite);
		m_countSprite = nullptr;

		return;
	}
	//OutputDebugStringA("aftSetGameScene\n");

	//スプライト変更処理
	//OutputDebugStringA("bforSpriteChange\n");
	if (m_countSprite != nullptr) {
		if (m_timer <= COUNTDOWN_PHASE1_START && m_timer > COUNTDOWN_PHASE1_END && changeCount == COUNTDOWN_PHASE_ONE) {
			//OutputDebugStringA("Image3\n");
			m_countSprite->Init("Assets/Image/3.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			alpha = 0.0f;
			scaleRate = COUNTDOWN_START_SCALE;
			fadeOut = false;
			changeCount++;
			m_soundFlag = true;
			//OutputDebugStringA("afterImage3\n");
		}
		else if (m_timer <= COUNTDOWN_PHASE2_START && m_timer > COUNTDOWN_PHASE2_END && changeCount == COUNTDOWN_PHASE_TWO) {
			//OutputDebugStringA("Image2\n");
			m_countSprite->Init("Assets/Image/2.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			alpha = 0.0f;
			scaleRate = COUNTDOWN_START_SCALE;
			fadeOut = false;
			changeCount++;
			m_soundFlag = true;
			//OutputDebugStringA("afterImage2\n");
		}
		else if (m_timer <= COUNTDOWN_PHASE3_START && m_timer > COUNTDOWN_PHASE3_END && changeCount == COUNTDOWN_PHASE_THREE) {
			DeleteGO(m_countSprite);
			m_countSprite = nullptr;
			m_countSprite = NewGO<SpriteRender>(20, "count");
			m_countSprite->SetColor(COUNTDOWN_START_COLOR);
			m_countSprite->SetPivot(COUNTDOWN_PIVOT);
			m_countSprite->SetPosition(COUNTDOWN_POS);
			//OutputDebugStringA("Image1\n");
			m_countSprite->Init("Assets/Image/1.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			m_soundFlag = true;
			//OutputDebugStringA("1\n");
			alpha = 0.0f;
			//OutputDebugStringA("2\n");
			scaleRate = COUNTDOWN_START_SCALE;
			//OutputDebugStringA("3\n");
			fadeOut = false;
			//OutputDebugStringA("4\n");
			changeCount++;
			///OutputDebugStringA("afterImage1\n");
		}
		else if (m_timer <= COUNTDOWN_PHASE_FINAL_START && m_timer > COUNTDOWN_PHASE_FINAL_END && changeCount == COUNTDOWN_PHASE_END) {
			//OutputDebugStringA("ImageStart\n");
			m_countSprite->Init("Assets/Image/start.dds", COUNTDOWN_SPRITE_WIDTH, COUNTDOWN_SPRITE_HEIGHT);
			alpha = 0.0f;
			scaleRate = COUNTDOWN_START_SCALE;
			fadeOut = false;
			changeCount++;
			m_soundFlag = true;
			//OutputDebugStringA("afterImageStart\n");
		}
	}

	//OutputDebugStringA("aftSpriteChange\n");

	//カウントダウン処理(スプライト)
	//フェードイン、アウト処理
	if (fadeOut == false) {
		alpha += gameTime;
	}
	else {
		alpha -= gameTime;
	}

	if (alpha > 1.0f) {
		alpha = 1.0f;
		fadeOut = true;
	}
	else if (alpha < 0.0f) {
		alpha = 0.0f;
	}

	//スケール縮小処理
	scaleRate -= COUNTDOWN_SHRINK_SPEED;
	if (scaleRate < 1.0f) {
		scaleRate = 1.0f;
	}

	//OutputDebugStringA("bforSound\n");
	//開始時のカウントダウンに応じて音を鳴らす。

	if (m_timer < COUNTDOWN_PHASE_FINAL_START && m_soundFlag == true) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/basketball_buzzer1.wav", false);
		se->SetVolume(SE_VOLUME);
		se->Play(false);
		m_soundFlag = false;
	}
	else if (m_timer < COUNTDOWN_PHASE1_START && m_timer > COUNTDOWN_PHASE_FINAL_START && m_soundFlag == true) {
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/Time.wav", false);
		se->SetVolume(SE_VOLUME);
		se->Play(false);
		m_soundFlag = false;
	}
	///OutputDebugStringA("aftSound\n");

	Vector3 countScale = { scaleRate,scaleRate,1.0f };
	m_countDownColor.w = alpha;
	m_countSprite->SetColor(m_countDownColor);
	m_countSprite->SetScale(countScale);

	//OutputDebugStringA("\n");
}