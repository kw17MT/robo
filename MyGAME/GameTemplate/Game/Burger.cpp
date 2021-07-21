#include "stdafx.h"
#include "Burger.h"
#include "Kitchen.h"
#include "Counter.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "TrashCan.h"
#include "SoundSource.h"
#include "effect/Effect.h"
#include <string>

namespace {
	const Vector3 EFFECT_SCALE = { 10.0f,10.0f,10.0f };
	const int KITCHEN_NAME_SIZE = 10;
	const int PLAYER_NAME_SIZE = 9;
	const int TRASHCAN_NAME_SIZE = 11;
	const int COUNTER_NAME_SIZE = 10;
	const int NONE = 9;
	const int DEFAULT_DECREMENT_TIME = 20;
	const float SPACE_BETWEEN_KITCHEN_TO_BURGER = 100.0f;
	const float AJUST_HEIGHT = 50.0f;
	const float AJUST_SPEED_TO_FOLLOW_PLAYER = 90.0f;
	const float DISTANCE_BETWEEN_PLAYER_TO_BURGER = 150.0f;
	const float SE_VOLUME = 2.0f;
}

Burger::~Burger()
{
	DeleteGO(m_skinModelRender);
}

bool Burger::Start()
{
	//モデルの初期化
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画用モデルの初期化
	m_skinModelRender->Init("Assets/modelData/food/Burger.tkm", nullptr, enModelUpAxisZ, m_position);
	//シャドウキャスター用の初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/food/Burger.tkm", nullptr, enModelUpAxisZ, m_position);
	//モデルの拡大
	m_skinModelRender->SetScale(m_burgerScale);

	//バーガーの番号を取得
	int endNo = m_burgerNo;
	//string型に変えてcharに変換するための準備をする。
	std::string endNo_string = std::to_string(endNo);
	//不変箇所を定義
	char playerName[PLAYER_NAME_SIZE] = "player0";
	char trashcanName[TRASHCAN_NAME_SIZE] = "trashcan0";
	char counterName[COUNTER_NAME_SIZE] = "counter0";
	char kitchenName[KITCHEN_NAME_SIZE] = "kitchen0";
	//末端番号だけを追加する
	strcat_s(kitchenName, endNo_string.c_str());
	strcat_s(playerName, endNo_string.c_str());
	strcat_s(trashcanName, endNo_string.c_str());
	strcat_s(counterName, endNo_string.c_str());
	//所定のキッチンを設定後、座標をとってきてセットする。
	Kitchen* kitchen = FindGO<Kitchen>(kitchenName);
	Vector3 kitchenPos = kitchen->GetKitchenPos();
	kitchenPos.y += SPACE_BETWEEN_KITCHEN_TO_BURGER;
	SetPosition(kitchenPos);

	//作られた側のプレイヤー、ゴミ箱、カウンターを探す。
	m_player = FindGO<Player>(playerName);
	m_trashCan = FindGO<TrashCan>(trashcanName);
	m_counter = FindGO<Counter>(counterName);

	//ハンバーガーが出てきたときのエフェクト
	Effect* m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/kirakira.efk");
	m_effect->Play();
	m_effect->SetScale(EFFECT_SCALE);
	m_effect->SetPosition(m_position);

	m_skinModelRender->SetApplyBlur(true);

	return true;
}

//プレイヤーがバーガーを持つ。
void Burger::GrabBurger()
{
	//プレイヤーの位置座標を取得
	Vector3 plPos = m_player->GetPosition();
	//プレイヤーの通常移動速度を取得
	Vector3 plSpeed = m_player->GetNormalMoveSpeed();
	//プレイヤーの移動速度を上げる
	plSpeed *= AJUST_SPEED_TO_FOLLOW_PLAYER;

	//プレイヤーとハンバーガーの距離を測る
	Vector3 playerToBurger_vec = plPos - m_position;
	float playerToBurger = playerToBurger_vec.Length();

	//Aボタンを押してプレイヤーとバーガーの距離が一定以下なら、バーガーを持つ準備をする。
	if (g_pad[m_burgerNo]->IsTrigger(enButtonA) && playerToBurger < DISTANCE_BETWEEN_PLAYER_TO_BURGER && m_player->GetPlayerState() != enHaveBurger) {
		//プレイヤーの状態を、ハンバーガー所持状態にする
		m_player->SetPlayerState(enHaveBurger);
		//音を鳴らす
		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/poka01.wav", false);
		se->SetVolume(SE_VOLUME);
		se->Play(false);
	}
	//バーガーの位置をプレイヤーの前に持ってくる。
	if (m_player->GetPlayerState() == enHaveBurger) {
		//プレイヤーの移動方向にハンバーガーを持ってくる。
		plPos += plSpeed;
		m_beHadPos = plPos;
		//高さを調整。
		m_beHadPos.y += AJUST_HEIGHT;
		//カウンタ―に置かれていないとき、プレイヤーの手元に来るように位置調整。
		if (m_putOnCounter == false) {
			m_position = m_beHadPos;
		}
	}
}

void Burger::ClearNo()
{
	//カウンターに保存されている積んだ具材の”数”を０に。
	m_counter->SetStack0();
	//プレイヤーに保存している積んだ具材を何もない状態（９）で初期化する。
	for (int i = 0;i < m_player->GetMaxNumCanSaveGuzaiType(); i++) {
		m_player->ClearSpecificGuzaiNo(i);
	}
}

void Burger::SetOnTrashCan() 
{
	//ゴミ箱との距離が一定以下でAボタンが押されたら
	if (g_pad[m_burgerNo]->IsTrigger(enButtonA)
		&& m_trashCan->GetCanTrash()) {
		//ハンバーガーがゴミ箱にセットされている状態にする。
		m_isSetOnTrashCan = true;
	}
	//ハンバーガーがゴミ箱にセットされていたら
	if (m_isSetOnTrashCan == true) {
		m_decrementTime--;
		//遅延させた後消す
		if (m_decrementTime <= 0) {
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/dumping.wav", false);
			se->SetVolume(SE_VOLUME);
			se->Play(false);
			//
			//m_burgerExist = false;
			m_player->SetPlayerState(enNothing);
			m_decrementTime = DEFAULT_DECREMENT_TIME;
			ClearNo();

			m_trashCan->ChangeMovingState(true);

			//ハンバーガーを消す
			DeleteGO(this);
		}
		//消すまではゴミ箱の上で待機させる。
		m_position = m_trashCan->GetPosition();
		m_position.y += AJUST_HEIGHT;
	}
}

void Burger::Update()
{
	GrabBurger();
	
	SetOnTrashCan();

	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_burgerScale);
}
