#include "stdafx.h"
#include "Kitchen.h"
#include "Burger.h"
#include "Guzai.h"
#include "Counter.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "SoundSource.h"
#include "Meter.h"
#include "MenuTimer.h"

namespace
{
	const Vector3 MOVE_SPEED_ZERO = Vector3::Zero;

	const int PLAYER_NAME_SIZE = 9;
	const int COUNTER_NAME_SIZE = 10;
	const int BURGER_NAME_SIZE = 9;
	const int AJUST_METER_POS_X_No0 = 200;
	const int AJUST_METER_POS_X_No1 = 100;
	const int AJUST_METER_POS_Y = 300;
	const int AJUST_METER_POS_Z = 100;
	const int HAVE_GUZAI = 1;
	const int NOTHING = 0;
	const int FULL_KITCHEN = -1;
	const int MAX_STACK_NUM = 5;
	const int KITCHEN_NUMBER_0 = 0;
	const int KITCHEN_NUMBER_1 = 1;
	const int DEFAULT_DELAY_NUMBER = 60;

	const float SCALE_DECREASE_SPEED = 1.4f / 60.0f;
	const float GUZAI_DOWN_SPEED = 0.7f;
	const float SE_VOLUME = 1.5f;
	const float DISTANCE_BETWEEN_PLAYER_TO_KITCHEN = 100.0f;
}

Kitchen::~Kitchen()
{
	DeleteGO(m_skinModelRender);
	
	//バーガーの消去
	if (bur != nullptr) {
		DeleteGO(bur);
	}

	//スタックした具材が残っていたら消去
	for (int i = 0; i < MAX_STACK_NUM; i++) {
		if (m_stackedGuzai[i] != nullptr) {
			DeleteGO(m_stackedGuzai[i]);
		}
	}

	if (m_soundFlag) {
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
	char counterName[COUNTER_NAME_SIZE] = "counter0";
	//末端番号だけを追加する
	strcat_s(playerName, endNo_string.c_str());
	strcat_s(counterName, endNo_string.c_str());
	//必要なデータの探索と確保
	m_player = FindGO<Player>(playerName);
	m_counter = FindGO<Counter>(counterName);

	//モデルデータの初期化
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画モデルの初期化
	m_skinModelRender->InitForRecieveShadow("Assets/modelData/kitchen/kitchen.tkm", nullptr, enModelUpAxisZ, m_position);
	//シャドウキャスト用の初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/kitchen/kitchen.tkm", nullptr, enModelUpAxisZ, m_position);
	//拡大率調整
	m_skinModelRender->SetScale(m_scale);

	return true;
}

//積んでいる具材の全消去と、それに関するパラメータの初期化
void Kitchen::Delete()
{
	//積んでいる具材の消去
	for (int i = 0;i < m_stack; i++) {
		//積んでいる具材の消去
		DeleteGO(m_stackedGuzai[i]);
	}
	//キッチンに積んでいた具材の数をカウンターの方に移す
	m_counter->SetStackNum(m_stack);
	//キッチンに積んでいる具材数を０で初期化
	m_stack = 0;
}

// Delayは必要
// ないとエラー
void Kitchen::BornBurger()
{
	//プレイヤーとキッチンとの間の距離
	Vector3 preDistance = m_player->GetPosition() - m_position;
	float distance = preDistance.Length();

	//具材を一つ以上積んでいて、Bボタンを長押し、プレイヤーは調理していない（何もしていない）とき
	if (m_stack > 0 && g_pad[m_kitchenNo]->IsPress(enButtonB) && m_isPlayerCookingOnKitchen == false && distance < DISTANCE_BETWEEN_PLAYER_TO_KITCHEN && m_player->GetPlayerState() <= NOTHING) {
		m_delay--;
		//プレイヤーが動けないようにする。
		m_player->StopMove(true);
		m_player->SetMoveSpeed(MOVE_SPEED_ZERO);
		if (m_soundFlag == false) {
			//調理の進み具合のメーターを表示
			m_meter = NewGO<Meter>(0);
			if (m_kitchenNo == KITCHEN_NUMBER_0) {
				//基本となる位置をキッチンの場所にする。
				Vector3 pos = m_position;
				//位置の調整を加える
				pos.x -= AJUST_METER_POS_X_No0;
				pos.y += AJUST_METER_POS_Y;
				pos.z += AJUST_METER_POS_Z;
				//メーターの場所を設定。
				m_meter->SetPosition(pos);
			}
			if (m_kitchenNo == KITCHEN_NUMBER_1) {
				Vector3 pos = m_position;
				pos.x += AJUST_METER_POS_X_No1;
				pos.y += AJUST_METER_POS_Y;
				pos.z += AJUST_METER_POS_Z;

				m_meter->SetPosition(pos);
			}
			
			//音を鳴らす
			m_soundSource = NewGO<CSoundSource>(0);
			m_soundSource->Init(L"Assets/sound/cutting_a_onion_speedy.wav", false);
			m_soundSource->SetVolume(SE_VOLUME);
			m_soundSource->Play(true);
			m_soundFlag = true;
		}
		//メーターを少しずつ減らす
		Vector3 scale = m_meter->GetScale();
		scale.x -= SCALE_DECREASE_SPEED;
		m_meter->SetScale(scale);
		if (m_delay == 0) {
			//音が出ていれば消す。
			if (m_soundFlag == true) {
				//メーターを消す
				DeleteGO(m_meter);
				DeleteGO(m_soundSource);
				m_soundFlag = false;
			}
			//ここで具材を持っていることにして、ハンバーガーができているとき具材をとれないようにしておく。
			m_player->SetPlayerState(HAVE_GUZAI);
			//削除フラグを立てる。
			m_isPlayerCookingOnKitchen = true;
			//遅延する時間を元に戻す
			m_delay = DEFAULT_DELAY_NUMBER;
		}
	}
	else {
		if (m_isPlayerCookingOnKitchen == false) {
			m_delay = DEFAULT_DELAY_NUMBER;
			m_player -> StopMove(false);
		}

		//音が出ていれば消す。
		if (m_soundFlag == true) {
			//メーターを消す。
			DeleteGO(m_meter);
			DeleteGO(m_soundSource);
			m_soundFlag = false;
		}
	}
	//削除フラグが立っているとき…
	if (m_isPlayerCookingOnKitchen == true) {
		m_player->SetPlayerState(HAVE_GUZAI);
		m_delay--;
		for (int i = 0; i < m_stack; i++) {
			m_slidePos = m_stackedGuzai[i]->GetPosition();
			//具材のy座標を(速度×積んでる段数)で下げる。
			m_slidePos.y -= GUZAI_DOWN_SPEED * i;
			m_stackedGuzai[i]->SetPosition(m_slidePos);
		}
		if (m_delay == 0) {
			//キッチンについている具材を全部消去
			Delete();

			if (m_kitchenNo == 0) {
				bur = NewGO<Burger>(0, "burger00");
			}
			if (m_kitchenNo == 1) {
				bur = NewGO<Burger>(0, "burger01");
			}

			bur->SetBurgerNo(m_kitchenNo);
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/thi-n.wav", false);
			se->SetVolume(SE_VOLUME);
			se->Play(false);

			m_player->StopMove(false);
			m_isPlayerCookingOnKitchen = false;
			m_delay = DEFAULT_DELAY_NUMBER;
		}
	}
}

void Kitchen::ClearNo()
{
	for (int i = 0;i < m_stack; i++) {
		m_player->ClearSpecificGuzaiNo(i);
	}
}

void Kitchen::TakePlayerLastestGuzai()
{
	//取る処理
	//1層以上積まれていたらとれるようにする。
	if (m_stack >= 1 && m_distance <= DISTANCE_BETWEEN_PLAYER_TO_KITCHEN) {
		if (g_pad[m_kitchenNo]->IsTrigger(enButtonA) && /*m_kitchenNo == 1 &&*/ m_canGrab == true && m_player->GetPlayerState() <= NOTHING) {
			//この具材は一回キッチンから帰ってきたか
			m_stackedGuzai[m_stack - 1]->SetReturnedState(true);
			//持たれているか
			m_stackedGuzai[m_stack - 1]->SetisHadState(true);
			//どちらのプレイヤーに持たれているか
			m_stackedGuzai[m_stack - 1]->SetWhichPlayerGet(m_kitchenNo);
			//一番上だった具材の番号を９にして何も入っていない状態にする。
			m_player->ClearSpecificGuzaiNo(m_stack - 1);
			//この具材はキッチンに置かれているか
			m_stackedGuzai[m_stack - 1]->SetPutState(false);
			//現在のスタック数、次のスタック番号を1減らす
			m_stack--;
			//プレイヤーは具材を持っている状態にする。
			m_player->SetPlayerState(HAVE_GUZAI);
		}
	}

	//キッチンに5個以上具材があると"コンベアからは"取れないようにする。
	if (m_stack >= m_maxStack) {
		m_player->SetPlayerState(FULL_KITCHEN);
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
	}
	
	//毎フレームプレイヤーとキッチンの距離を測る
	Vector3 preDistance = m_player->GetPosition() - m_position;
	m_distance = preDistance.Length();

	//キッチンに1個以上具材が乗っていてプレイヤーが側でAボタンを押したら一番上の具材を渡す。
	TakePlayerLastestGuzai();

	//キッチンでプレイヤーがハンバーガーを作る処理
	BornBurger();

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_scale);
}