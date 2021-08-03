#include "stdafx.h"
#include "Counter.h"
#include "Burger.h"
#include "Kitchen.h"
#include "Score.h"
#include "CLevel2D.h"
#include "PlayerGene.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "SpriteRender.h"
#include "SoundSource.h"
#include "GameDirector.h"
#include "effect/Effect.h"
#include "DishManager.h"

namespace
{
	const Vector3 JUDGEMARK_LEFT_POS = { -95.0f,-255.0f,0.0f };
	const Vector3 JUDGEMARK_RIGHT_POS = { -155.0f,-255.0f,0.0f };

	const int ROWS_NUM = 4;						//行
	const int COLUMNS_NUM = 5;					//列
	const int GUZAI_TYPE_NONE = 2;
	const int KITCHEN_NAME_SIZE = 10;
	const int PLAYER_NAME_SIZE = 9;
	const int TRASHCAN_NAME_SIZE = 11;
	const int BURGER_NAME_SIZE = 9;
	const int SPRITE_HEIGHT = 100;
	const int SPRITE_WIDTH = 100;
	const int SPRITE_CHECK_SCALE = 30;
	const int SPRIRE_BATSU_SCALE = 40;
	const int COUNTER_NUMBER_ZERO = 0;
	const int COUNTER_NUMBER_ONE = 1;
	const int STACK_EMPTY = 9;
	const int MAX_DELAY_NUM = 60;
	const int MAX_STACK_NUM = 5;
	const int CORRECT = 1;
	const int FAULT = 0;
	const int NOTHING = 2;
	const int SPRITE_MAX_TIMER = 60;
	const int GUZAI_CORRECT = 1;
	const int GUZAI_FAILE = 0;
	const int BONUS_RATIO = 2;

	const float DISTANCE_TO_FIND_COUNTER = 100.0f;
	const float SE_VOLUME = 0.7f;
	const float MOVE_AMOUNT_LITTLE = 10.0f;
	const float AJUST_HEIGHT_FOR_JUDGEMARK = 30.0f;
	const float AJUST_POSITION_X_FOR_JUDGEMARK = 120.0f;
}


Counter::~Counter()
{
	//カウンターのモデルを消去
	DeleteGO(m_skinModelRender);
	
	//判定結果を表す画像を消去
	if (m_spriteJudge != nullptr) {
		DeleteGO(m_spriteJudge);
	}

	//メニュー上に出す正誤判定の画像を消去
	for (int i = 0; i < ROWS_NUM; i++) {
		for (int j = 0; j < COLUMNS_NUM; j++) {
			if (m_spriteCompareFlagFalse[i][j] || m_spriteCompareFlagTrue[i][j]) {
				DeleteGO(m_spriteCompare[i][j]);
			}
		}
	}
}

bool Counter::Start()
{
	//カウンターのモデルを初期化して、判定するハンバーガーの種類を格納していく
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//影を受け取り、画面上に出てくるモデルを初期化
	m_skinModelRender->InitForRecieveShadow("Assets/modelData/counter/counter.tkm", nullptr, enModelUpAxisZ, m_position);
	//影をつくるモデルの初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/counter/counter.tkm", nullptr, enModelUpAxisZ, m_position);
	//ハンバーガーのレシピを取得
	m_showHamBurgers[0] = enCheeseBurger;
	m_showHamBurgers[1] = enTomatoBurger;
	m_showHamBurgers[2] = enEggBurger;
	m_showHamBurgers[3] = enBasicBurger;
	m_showHamBurgers[4] = enDoubleBurger;
	m_showHamBurgers[5] = enVegetableBurger;
	m_showHamBurgers[6] = enBaconBurger;
	m_showHamBurgers[7] = enBLTBurger;
	m_showHamBurgers[8] = enOnionBurger;
	m_showHamBurgers[9] = enHighcalBurger;
	m_showHamBurgers[10] = enOnOnionBurger;

	//具材の積まれた判定を積まれていない状態にする
	for (int i = 0; i < ROWS_NUM; i++) {
		for (int j = 0; j < COLUMNS_NUM; j++) {
			m_guzaiJudge[i][j] = GUZAI_TYPE_NONE;
		}
	}

	//string型に変えてcharに変換するための準備をする。
	std::string endNo_string = std::to_string(m_counterNo);
	//不変箇所を定義
	char kitchenName[KITCHEN_NAME_SIZE] = "kitchen0";
	char playerName[PLAYER_NAME_SIZE] = "player0";
	char trashcanName[TRASHCAN_NAME_SIZE] = "trashcan0";
	//末端番号だけを追加する
	strcat_s(kitchenName, endNo_string.c_str());
	strcat_s(playerName, endNo_string.c_str());
	strcat_s(trashcanName, endNo_string.c_str());

	m_level2d = FindGO<CLevel2D>("clevel2d");
	//対応するプレイヤーのオブジェクトを取得
	m_player = FindGO<Player>(playerName);
	//対応するキッチンのオブジェクトを取得
	m_kitchen = FindGO<Kitchen>(kitchenName);

	//ハンバーガーのポジションをカウンターの位置にするため。
	m_burgerPos = m_position;

	return true;
}

//判別するところ
bool Counter::Judge()
{
	//プレイヤーが積み上げてきた具材の種類から、提出されたハンバーガーが正しいか判別する。
	//今回の関数内では第一引数の配列の要素全てにおいて変更する内容がないためconst_castを使用
	return m_level2d->GetIsMatchHamBurger(const_cast<int*>(m_player->GetPlayerStackedGuzais()), m_stackNum, m_counterNo + 1);
}

float Counter::CalcDistance(Vector3 pos1, Vector3 pos2)
{
	//地点１と地点２の距離を測る。
	Vector3 distance = pos1 - pos2;
	return distance.Length();
}

//バーガーを最終的に消してスコアを発生させる。
//カウンターに近いところでAボタンを押すといったん載せて消す。
void Counter::JudgeAndDelete()
{
	//プレイヤーの情報が取得できていなかったとき
	if (m_player == nullptr) {
		//string型に変えてcharに変換するための準備をする。
		std::string endNo_string = std::to_string(m_counterNo);
		//不変箇所を定義
		char playerName[PLAYER_NAME_SIZE] = "player0";
		//末端番号だけを追加する
		strcat_s(playerName, endNo_string.c_str());

		m_player = FindGO<Player>(playerName);
	}

	Vector3 plPos = m_player->GetPosition();

	//カウンターからプレイヤーの距離
	float playerToCounter = CalcDistance(plPos, m_position);

	//プレイヤーがバーガーをもっていたら
	if (m_player->GetPlayerState() == enHaveBurger) {
		//string型に変えてcharに変換するための準備をする。
		std::string endNo_string = std::to_string(m_counterNo);
		//不変箇所を定義
		char burgerName[BURGER_NAME_SIZE] = "burger0";
		//末端番号だけを追加する
		strcat_s(burgerName, endNo_string.c_str());
		//場所に対応したハンバーガーを探す
		Burger* burger = FindGO<Burger>(burgerName);

		//カウンターに置く準備
		//できたハンバーガーの組成をJudge関数で調べる。
		if (g_pad[m_counterNo]->IsTrigger(enButtonA) && playerToCounter < DISTANCE_TO_FIND_COUNTER) {
			burger->SetPutOnCounterState(true);
			//バーガーが合っている場合
			if (Judge() == true) {
				m_burgerCorrectFlag = true;
				//画像がまだ出ていないとき
				if (m_spriteFlag == false) {
					m_spriteJudge = NewGO<SpriteRender>(0);
					m_spriteJudge->Init("Assets/Image/BurgerJudgeMark/Correct.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
					//ポジションを対応させる
					if (m_counterNo == COUNTER_NUMBER_ONE) {
						//カウンタ―ナンバーが1の時（右の時）、x座標を反転させる。
						m_spritePos.x *= -1;
					}
					m_spriteJudge->SetPosition(m_spritePos);
					m_spriteFlag = true;
					//音を鳴らす
					CSoundSource* se = NewGO<CSoundSource>(0);
					se->Init(L"Assets/sound/crrect_answer2.wav", false);
					se->SetVolume(SE_VOLUME);
					se->Play(false);
					//次の具材No.を保存するため、９で初期化。
					m_player->SetGuzaiEmpty();
				}
			}
			else {
				m_burgerMistakeFlag = true;
				if (m_spriteFlag == false) {
					m_spriteJudge = NewGO<SpriteRender>(0);
					m_spriteJudge->Init("Assets/Image/BurgerJudgeMark/Miss.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
					if (m_counterNo == COUNTER_NUMBER_ONE) {
						//カウンタ―ナンバーが1の時（右の時）、x座標を反転させる。
						m_spritePos.x *= -1;
					}
					m_spriteJudge->SetPosition(m_spritePos);
					m_spriteFlag = true;

					//音を鳴らす
					CSoundSource* se = NewGO<CSoundSource>(0);
					se->Init(L"Assets/sound/blip01.wav", false);
					se->SetVolume(SE_VOLUME);
					se->Play(false);
					m_player->SetGuzaiEmpty();
				}
			}
		}

		//ハンバーガーが存在していて
		if (burger != nullptr) {
			//バーガーが合っている場合
			if (m_burgerCorrectFlag == true) {
				m_delay++;
				//バーガーを動かす。
				m_burgerPos.z += MOVE_AMOUNT_LITTLE;
				burger->SetPosition(m_burgerPos);
				//置いたら一定フレーム後に消去
				if (m_delay == MAX_DELAY_NUM) {
					//ここで積み上げてた具材の数をScoreに渡してあげる。
					Score* sco = FindGO<Score>("score");
					if (m_counterNo == COUNTER_NUMBER_ZERO) {
						if (m_bonusPoint == true) {
							sco->SetBasePoint01(m_stackNum * BONUS_RATIO);
							m_bonusPoint = false;
						}
						else {
							sco->SetBasePoint01(m_stackNum);
						}
					}
					if (m_counterNo == COUNTER_NUMBER_ONE) {
						if (m_bonusPoint == true) {
							sco->SetBasePoint02(m_stackNum * BONUS_RATIO);
							m_bonusPoint = false;
						}
						else {
							sco->SetBasePoint02(m_stackNum);
						}
					}

					//プレイヤーは何も持っていない状態にする。
					m_player->SetPlayerState(enNothing);
					//Delayを初期値に戻す
					m_delay = 0;
					//ハンバーガーの層数を0にもどす。
					m_stackNum = 0;
					//皿の動く向きを逆にさせる命令を発信する。
					DishManager::GetInstance().SetOrderChangeDirection(true);
					//次のハンバーガーのために、カウンターの座標に書き換える。
					m_burgerPos = m_position;
					//一応FALSEにしておく。
					m_burgerCorrectFlag = false;
					//ハンバーガーを消す。
					DeleteGO(burger);
				}
			}
			//バーガーが間違っている場合。
			if (m_burgerMistakeFlag == true) {
				m_delay++;
				//バーガーを動かす。
				if (m_counterNo == COUNTER_NUMBER_ZERO) {
					m_burgerPos.x += MOVE_AMOUNT_LITTLE;
				}
				if (m_counterNo == COUNTER_NUMBER_ONE) {
					m_burgerPos.x -= MOVE_AMOUNT_LITTLE;
				}
				burger->SetPosition(m_burgerPos);

				if (m_delay == MAX_DELAY_NUM) {
					m_player->SetPlayerState(enNothing);
					DeleteGO(burger);
					m_delay = 0;

					m_burgerMistakeFlag = false;
					m_stackNum = 0;
					m_burgerPos = m_position;
				}
			}
		}
	}
}

void Counter::HamBurgerCompare()
{
	const int one = 1;

	CLevel2D* l2 = FindGO<CLevel2D>("clevel2d");
	//1P側の処理
	if (m_counterNo == COUNTER_NUMBER_ZERO) {
		for (int i = 2; i > m_counterNo; i--) {
			HamBurger hamburger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			//具材をキッチンに積んでいないとき…
			for (int k = 0; k < MAX_STACK_NUM; k++) {
				if (m_player->GetPlayerStackedGuzais(k) == STACK_EMPTY) {
					//画像が出ていれば消す
					if (m_spriteCompareFlagTrue[i + one][k] == true || m_spriteCompareFlagFalse[i + one][k] == true) {
						DeleteGO(m_spriteCompare[i + one][k]);
						m_spriteCompareFlagTrue[i + one][k] = false;
						m_spriteCompareFlagFalse[i + one][k] = false;
					}
				}
			}
			//メニューと合っているか調べる
			for (int j = 0; j < hamburger.size(); j++) {
				//積んでなければ何もしない。
				if (m_player->GetPlayerStackedGuzais(j) == STACK_EMPTY) {
					m_guzaiJudge[i + one][j] = NOTHING;
				}
				else {
					if (m_player->GetPlayerStackedGuzais(j) == hamburger[j]) {
						//メニューと一致
						m_guzaiJudge[i + one][j] = CORRECT;
					}
					else {
						//メニューと不一致
						m_guzaiJudge[i + one][j] = FAULT;
					}
				}

				Vector3 SetPos = JUDGEMARK_LEFT_POS ;
				SetPos.x += i * AJUST_POSITION_X_FOR_JUDGEMARK;
				SetPos.y += j * AJUST_HEIGHT_FOR_JUDGEMARK;
				SetPos.y += l2->GetSlideAmount(i);
				//メニューと一致しているかで決める。
				switch (m_guzaiJudge[i+ one][j])
				{
					
				case GUZAI_FAILE: {
					//一致の画像が出ていれば消す。
					if (m_spriteCompareFlagTrue[i + one][j] == true) {
						DeleteGO(m_spriteCompare[i + one][j]);
						m_spriteCompareFlagTrue[i + one][j] = false;
					}
					//不一致の画像が出ていなければ出す。
					if (m_spriteCompareFlagFalse[i + one][j] == false) {
						m_spriteCompare[i + one][j] = NewGO<SpriteRender>(10);
						m_spriteCompare[i + one][j]->Init("Assets/Image/JudgeMark/Batsu_White_Black512.dds", SPRIRE_BATSU_SCALE, SPRIRE_BATSU_SCALE);
						m_spriteCompare[i + one][j]->SetPosition(SetPos);
						m_spriteCompareFlagFalse[i + one][j] = true;
					}
				}break;
				case GUZAI_CORRECT: {
					//不一致の画像が出ていれば消す。
					if (m_spriteCompareFlagFalse[i + one][j] == true) {
						DeleteGO(m_spriteCompare[i + one][j]);
						m_spriteCompareFlagFalse[i + one][j] = false;
					}
					//一致の画像が出ていなければ出す。
					if (m_spriteCompareFlagTrue[i + one][j] == false) {
						m_spriteCompare[i + one][j] = NewGO<SpriteRender>(10);
						m_spriteCompare[i + one][j]->Init("Assets/Image/JudgeMark/Check_White_Red512.dds", SPRITE_CHECK_SCALE, SPRITE_CHECK_SCALE);
						m_spriteCompare[i + one][j]->SetPosition(SetPos);
						m_spriteCompareFlagTrue[i + one][j] = true;
					}
				}break;
				default:
					break;
				}
				//チェックマークの座標を更新。
				if (m_spriteCompareFlagFalse[i + one][j] == true || m_spriteCompareFlagTrue[i + one][j] == true) {
					m_spriteCompare[i + one][j]->SetPosition(SetPos);
				}
				//すでに出したチェック画像は、バーガーの段数を超えて処理する。
				for (int l = static_cast<int>(hamburger.size()); l < MAX_STACK_NUM; l++) {
					Vector3 SetPos = JUDGEMARK_LEFT_POS;
					SetPos.x += i * AJUST_POSITION_X_FOR_JUDGEMARK;
					SetPos.y += l * AJUST_HEIGHT_FOR_JUDGEMARK;
					SetPos.y += l2->GetSlideAmount(i);
					if (m_player->GetPlayerStackedGuzais(l) != STACK_EMPTY) {
						if (m_spriteCompareFlagFalse[i + one][l] == true || m_spriteCompareFlagTrue[i + one][l] == true) {

							DeleteGO(m_spriteCompare[i + one][l]);
							m_spriteCompareFlagFalse[i + one][l] = false;
							m_spriteCompareFlagTrue[i + one][l] = false;
						}
					}
				}
			}
		}
	}
	//2P側の処理
	if (m_counterNo == COUNTER_NUMBER_ONE) {
		for (int i = 0; i < m_counterNo + 1; i++) {
			HamBurger hamburger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			for (int k = 0; k < MAX_STACK_NUM; k++) {
				if (m_player->GetPlayerStackedGuzais(k) == STACK_EMPTY) {
					if (m_spriteCompareFlagTrue[i][k] == true || m_spriteCompareFlagFalse[i][k] == true) {
						DeleteGO(m_spriteCompare[i][k]);
						m_spriteCompareFlagTrue[i][k] = false;
						m_spriteCompareFlagFalse[i][k] = false;
					}
				}
			}
			for (int j = 0; j < hamburger.size(); j++) {
				if (m_player->GetPlayerStackedGuzais(j) == STACK_EMPTY) {
					m_guzaiJudge[i][j] = NOTHING;
				}
				else {
					if (m_player->GetPlayerStackedGuzais(j) == hamburger[j]) {
						m_guzaiJudge[i][j] = CORRECT;
					}
					else {
						m_guzaiJudge[i][j] = FAULT;
					}
				}
				Vector3 SetPos = JUDGEMARK_RIGHT_POS;
				SetPos.x += i * AJUST_POSITION_X_FOR_JUDGEMARK;
				SetPos.y += j * AJUST_HEIGHT_FOR_JUDGEMARK;
				SetPos.y += l2->GetSlideAmount(i);
				switch (m_guzaiJudge[i][j])
				{
				case GUZAI_FAILE: {
					if (m_spriteCompareFlagTrue[i][j] == true) {
						DeleteGO(m_spriteCompare[i][j]);
						m_spriteCompareFlagTrue[i][j] = false;
					}
					if (m_spriteCompareFlagFalse[i][j] == false) {
						m_spriteCompare[i][j] = NewGO<SpriteRender>(10);
						m_spriteCompare[i][j]->Init("Assets/Image/JudgeMark/Batsu_White_Black512.dds", SPRIRE_BATSU_SCALE, SPRIRE_BATSU_SCALE);
						m_spriteCompare[i][j]->SetPosition(SetPos);
						m_spriteCompareFlagFalse[i][j] = true;
					}
				}break;
				case GUZAI_CORRECT: {
					if (m_spriteCompareFlagFalse[i][j] == true) {
						DeleteGO(m_spriteCompare[i][j]);
						m_spriteCompareFlagFalse[i][j] = false;
					}
					if (m_spriteCompareFlagTrue[i][j] == false) {
						m_spriteCompare[i][j] = NewGO<SpriteRender>(10);
						m_spriteCompare[i][j]->Init("Assets/Image/JudgeMark/Check_White_Blue512.dds", SPRITE_CHECK_SCALE, SPRITE_CHECK_SCALE);
						m_spriteCompare[i][j]->SetPosition(SetPos);
						m_spriteCompareFlagTrue[i][j] = true;
					}
				}break;
				default:
					break;
				}
				if (m_spriteCompareFlagFalse[i][j] == true || m_spriteCompareFlagTrue[i][j] == true) {
					m_spriteCompare[i][j]->SetPosition(SetPos);
				}
				for (int l = static_cast<int>(hamburger.size()); l < MAX_STACK_NUM; l++) {
					Vector3 SetPos = JUDGEMARK_RIGHT_POS;
					SetPos.x += i * AJUST_POSITION_X_FOR_JUDGEMARK;
					SetPos.y += l * AJUST_HEIGHT_FOR_JUDGEMARK;
					SetPos.y += l2->GetSlideAmount(i);
					if (m_spriteCompareFlagFalse[i][l] == true || m_spriteCompareFlagTrue[i][l] == true) {
						if (m_player->GetPlayerStackedGuzais(l) != STACK_EMPTY) {
							if (m_spriteCompareFlagFalse[i][l] == true || m_spriteCompareFlagTrue[i][l] == true) {

								DeleteGO(m_spriteCompare[i][l]);
								m_spriteCompareFlagFalse[i][l] = false;
								m_spriteCompareFlagTrue[i][l] = false;
							}
						}
					}
				}
			}
		}
	}
}

void Counter::Update()
{
	//提出されたハンバーガーを判定し、削除する
	JudgeAndDelete();
	//今作成しているハンバーガーの組成の正誤判定
	HamBurgerCompare();
	
	//判定結果の正誤表示の画像が出ていたら
	if (m_spriteFlag == true) {
		m_spriteTime++;
		//一定時間後消去
		if (m_spriteTime > SPRITE_MAX_TIMER) {
			DeleteGO(m_spriteJudge);
			m_spriteFlag = false;
			m_spriteTime = 0;
		}
	}
	//位置を更新
	m_skinModelRender->SetPosition(m_position);
}