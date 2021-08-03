#include "stdafx.h"
#include "CLevel2D.h"
#include "SpriteRender.h"
#include "MenuTimer.h"
#include "MissCounter.h"
#include "Counter.h"
#include "SoundSource.h"

namespace
{
	const Vector3 MENU_TIMER_LEFT_POS = { 480.0f,5.0f,750.0f };
	const Vector3 MENU_TIMER_RIGHT_POS = { -480.0f,5.0f,750.0f };
	const Vector3 MENU_SCALE = { 0.5f,0.5f,0.5f };

	const int SHOW_BURGER_RIGHT = 0;
	const int SHOW_BURGER_MIDDLE = 1;
	const int SHOW_BURGER_LEFT = 2;
	const int MENU_TIMER_LEFT = 0;
	const int MENU_TIMER_RIGHT = 1;
	const int MENU_TIMEOVER_LEFT = 0;
	const int MENU_TIMEOVER_RIGHT = 1;
	const int MENU_TIMER_NUM = 2;
	const int MENU_TIMEUP_LEFT = 0;
	const int MENU_TIMEUP_RIGHT = 1;
	const int COUNTER_NUMBER_ONE = 1;
	const int COUNTER_NUMBER_TWO = 2;
	const int SLIDE_RATE = 10.0f;

	const float SE_VOLUME = 2.0f;
	const float MAKE_MENU_BOTTOM = 350.0f;
}

CLevel2D::~CLevel2D()
{
	DeleteGO(m_missCounter);
	for (int i = 0; i < MENU_TIMER_NUM; i++) {
		DeleteGO(m_menuTimer[i]);
	}
	for (int i = 0; i < SHOW_HAMBURGER_NUMBER; i++) {
		DeleteGO(sprite[i]);
	}
}

bool CLevel2D::Start()
{
	m_counter00 = FindGO<Counter>("counter00");
	m_counter01 = FindGO<Counter>("counter01");
	//レベルを読み込む。
	//一番左が配列の3番目の要素、右が1番目の要素
	
	m_level2D.Init("Assets/level2D/level2D_add.casl", [&](Level2DObjectData& objdata) { 
		if (objdata.EqualObjectName("burger_cheese")) {
			//右側に出る（2P）
			m_spritePositions[SHOW_BURGER_RIGHT] = objdata.position;
			m_slidePos[SHOW_BURGER_RIGHT] = m_spritePositions[SHOW_BURGER_RIGHT];
			m_level2DObjectDatas[enCheeseBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_tomato")) {
			//中間に出る
			m_spritePositions[SHOW_BURGER_MIDDLE] = objdata.position;
			m_slidePos[SHOW_BURGER_MIDDLE] = m_spritePositions[SHOW_BURGER_MIDDLE];
			m_level2DObjectDatas[enTomatoBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_egg")) {
			//左側に出る（1p）
			m_spritePositions[SHOW_BURGER_LEFT] = objdata.position;
			m_slidePos[SHOW_BURGER_LEFT] = m_spritePositions[SHOW_BURGER_LEFT];
			m_level2DObjectDatas[enEggBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_basic")) {
			m_level2DObjectDatas[enBasicBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_double")) {
			m_level2DObjectDatas[enDoubleBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_vegetable")) {
			m_level2DObjectDatas[enVegetableBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_bacon")) {
			m_level2DObjectDatas[enBaconBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_blt")) {
			m_level2DObjectDatas[enBLTBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_onion")) {
			m_level2DObjectDatas[enOnionBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_high_cal")) {
			m_level2DObjectDatas[enHighcalBurger] = objdata;
			return true;
		}
		if (objdata.EqualObjectName("burger_on_onion")) {
			m_level2DObjectDatas[enOnOnionBurger] = objdata;
			return true;
		}
		else{
			return true;
		}
	});

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

	ShowHamBurger(enCheeseBurger, m_showHamBurgers[SHOW_BURGER_RIGHT]);
	ShowHamBurger(enTomatoBurger, m_showHamBurgers[SHOW_BURGER_MIDDLE]);
	ShowHamBurger(enEggBurger, m_showHamBurgers[SHOW_BURGER_LEFT]);

	m_randNum[SHOW_BURGER_RIGHT] = enCheeseBurger;
	m_randNum[SHOW_BURGER_MIDDLE] = enTomatoBurger;
	m_randNum[SHOW_BURGER_LEFT] = enEggBurger;

	//左側ゲージ
	m_menuTimer[MENU_TIMER_LEFT] = NewGO<MenuTimer>(0);
	Quaternion rot = Quaternion::Identity;
	//左右反転
	rot.SetRotationDegZ(180.0f);
	m_menuTimer[MENU_TIMER_LEFT]->SetRotation(rot);
	m_menuTimer[MENU_TIMER_LEFT]->SetPosition(MENU_TIMER_LEFT_POS);
	m_menuTimer[MENU_TIMER_LEFT]->SetNumber(MENU_TIMER_LEFT);
	//右側ゲージ
	m_menuTimer[MENU_TIMER_RIGHT] = NewGO<MenuTimer>(0);
	m_menuTimer[MENU_TIMER_RIGHT]->SetPosition(MENU_TIMER_RIGHT_POS);
	m_menuTimer[MENU_TIMER_RIGHT]->SetNumber(MENU_TIMER_RIGHT);


	m_missCounter = NewGO<MissCounter>(0);

	return true;
}

void CLevel2D::Update()
{
	//レベル2DクラスのSpriteの更新処理。

	for (int i = 0; i < SHOW_HAMBURGER_NUMBER; i++) {
		SpriteSet(i);
	}

	//プレイヤー1側のメニューの時間切れ
	if (m_slide[SHOW_BURGER_LEFT] == enStop) {
		if (m_menuTimer[0]->GetTimeUpState()) {
			//時間切れ中のフラグが立っていないとき…
			if (m_TimeUpSet[MENU_TIMEOVER_LEFT] == false) {
				//左のメニューの再抽選
				Roulette(SHOW_BURGER_LEFT);
				//1Pのミス数を1足す
				m_missCounter->AddPl1MissCount();
				//バツ印の画像を出す
				m_missCounter->ChangeMarkState(true);
				//時間切れのフラグを立てる
				m_TimeUpSet[MENU_TIMEUP_LEFT] = true;
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/blip01.wav", false);
				se->SetVolume(SE_VOLUME);
				se->Play(false);
			}

		}
	}
	//プレイヤー2の時間切れ
	if (m_slide[SHOW_BURGER_RIGHT] == enStop) {
		if (m_menuTimer[MENU_TIMEOVER_RIGHT]->GetTimeUpState()) {
			if (m_TimeUpSet[MENU_TIMEOVER_RIGHT] == false) {
				Roulette(SHOW_BURGER_RIGHT);
				m_missCounter->AddPl2MissCount();
				m_missCounter->ChangeMarkState(true);

				m_TimeUpSet[MENU_TIMEUP_RIGHT] = true;
				//音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/blip01.wav", false);
				se->SetVolume(SE_VOLUME);
				se->Play(false);
			}
		}
	}
	
	m_level2D.Update();
}

void CLevel2D::Render(RenderContext& rc)
{
	if (GameObjectManager::GetInstance()->GetRenderTypes() == 0)
	{
		m_level2D.Draw(rc);
	}
}

//ここのカウンターナンバーは１or２でいい。引数の時点でナンバーにインクリメントする。
bool CLevel2D::GetIsMatchHamBurger(int* numbers, int size, int counterNo)
{
	if (counterNo == COUNTER_NUMBER_ONE) {
		//メニューの左と中央を対象に比較開始
		for (int i = SHOW_BURGER_LEFT; i >= counterNo; i--)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			//長さ違ったら。
			if (size != hamBurger.size()) 
				//以下の処理しなーい。
			continue;
			
			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				int num = numbers[j];
				//具材が違ってたら。
				if (num != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				//中央のメニューが出来たら…
				if (i == SHOW_BURGER_MIDDLE) {
					//ボーナスポイントのフラグを立てる；
					m_counter00->SetBonusPoint(true);
				}
				//次に表示するハンバーガー決めるお！
				Roulette(i);
				return true;
			}
			
		}
	}

	if (counterNo == COUNTER_NUMBER_TWO) {
		//メニューの右と中央を対象に比較開始
		for (int i = 0; i < counterNo; i++)
		{
			//ハンバーガーのデータ持ってくるお。
			HamBurger hamBurger = GetHamBurgerFactory().GetHamBurger(m_showHamBurgers[i]);
			//長さ違ったら。
			if (size != hamBurger.size())
				//以下の処理しなーい。
				continue;

			//同じだお。
			bool isSame = true;
			for (int j = 0; j < hamBurger.size(); j++)
			{
				int num = numbers[j];
				//具材が違ってたら。
				if (num != hamBurger[j])
				{
					//違うお。
					isSame = false;
					break;
				}
			}
			//同じだったお。
			if (isSame == true)
			{
				if (i == SHOW_BURGER_MIDDLE) {
					m_counter01->SetBonusPoint(true);
				}
				//次に表示するハンバーガー決めるお！
				Roulette(i);

				return true;
			}
		}
	}
	//同じじゃなかったら、false以外ありえない。
	return false;
}

void CLevel2D::Roulette(int number)
{
	//TODO ここの乱数要修正？
	int rn = rand() % enHamBurgerNum;
	
	//中央のメニューと被らないようにメニューを決める
	if(number == SHOW_BURGER_MIDDLE){
		while (rn == m_randNum[SHOW_BURGER_RIGHT] || rn == m_randNum[SHOW_BURGER_LEFT])
		{
			rn = rand() % enHamBurgerNum;
		}
	}
	else {
		while (rn == m_randNum[SHOW_BURGER_MIDDLE])
		{
			rn = rand() % enHamBurgerNum;
		}
	}
	m_showHamBurgers[number] = EnHamBurger(rn);

	//カウンターに表示しているバーガーを伝える。
	m_randNum[number] = rn;
	
	//画像を動かす
	m_slide[number] = enSlideDown;
	
	//音を鳴らす
	m_slideSe[number] = NewGO<CSoundSource>(0);
	m_slideSe[number]->Init(L"Assets/sound/machine_rotation1.wav", false);
	m_slideSe[number]->SetVolume(SE_VOLUME);
	m_slideSe[number]->Play(true);
}

void CLevel2D::SpriteSet(int number)
{
	//移動フラグによって処理を分ける。
	switch (m_slide[number])
	{
	case enStop: {	//0なら動かない。
		sprite[number]->SetPosition(m_spritePositions[number]);
		//Roulette(number);
	}break;
	case enSlideUp: {	//1なら上にスライド
		m_slidePos[number].y += SLIDE_RATE;
		m_slideAmount[number] += SLIDE_RATE;
		sprite[number]->SetPosition(m_slidePos[number]);

		//画像の位置が元の位置に戻ったら…
		if (m_slidePos[number].y == m_spritePositions[number].y) {
			//2P側のメニュー
			if (number == enStop) {
				//メニュータイマーを元に戻す
				m_menuTimer[MENU_TIMER_RIGHT]->ResetTimerParam(MENU_TIMER_RIGHT);
				m_menuTimer[MENU_TIMER_RIGHT]->SetTimeUpState(false);
				//時間切れフラグを元に戻す。
				m_TimeUpSet[MENU_TIMEUP_RIGHT] = false;
			}
			//1P側のメニュー
			else if (number == enSlideDown) {
				m_menuTimer[MENU_TIMER_LEFT]->ResetTimerParam(MENU_TIMER_LEFT);
				//バツを付けたのでFALSEにもどしてやる
				m_menuTimer[MENU_TIMER_LEFT]->SetTimeUpState(false);
				m_TimeUpSet[MENU_TIMEUP_LEFT] = false;
			}
			//スライドフラグ0に変更。
			m_slide[number] = enStop;
			DeleteGO(m_slideSe[number]);
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/button03b.wav", false);
			se->SetVolume(SE_VOLUME);
			se->Play(false);
		}
	}break;
	case enSlideDown: {	//2なら下にスライド。
		m_slidePos[number].y -= SLIDE_RATE;
		m_slideAmount[number] -= SLIDE_RATE;
		sprite[number]->SetPosition(m_slidePos[number]);
		//提出とタイムアップが重ならないようにする。
		if (number == enSlideDown) {
			m_menuTimer[MENU_TIMER_LEFT]->SetTimeUpState(false);
		}
		if (number == enStop) {
			m_menuTimer[MENU_TIMER_RIGHT]->SetTimeUpState(false);
		}
		//画像の位置が一定まで下がったら。
		if (m_slidePos[number].y < m_spritePositions[number].y - MAKE_MENU_BOTTOM) {
			//カウンターに表示しているバーガーを伝える。
			if (number == enSlideDown) {
				m_counter00->m_showHamBurgers[number] = EnHamBurger(m_randNum[number]);
			}
			else if (number == enStop) {
				m_counter01->m_showHamBurgers[number] = EnHamBurger(m_randNum[number]);
			}
			else if (number == enSlideUp) {
				m_counter00->m_showHamBurgers[number] = EnHamBurger(m_randNum[number]);
				m_counter01->m_showHamBurgers[number] = EnHamBurger(m_randNum[number]);
			}
			//スライドフラグを1に変更。
			m_slide[number] = enSlideUp;
			//メニュー画像を更新。
			ShowHamBurger(number, m_showHamBurgers[number]);
		}
	}break;
	default:
		break;
	}
}

void CLevel2D::ShowHamBurger(int number, EnHamBurger enHamBurger)
{
	if (sprite[number] != nullptr)
	{
		DeleteGO(sprite[number]);
	}

	Level2DObjectData& objData = m_level2DObjectDatas[enHamBurger];
	sprite[number] = NewGO<SpriteRender>(2);
	sprite[number]->Init(objData.ddsFilePath, objData.width, objData.height);
	sprite[number]->SetScale(MENU_SCALE);
	//ハンバーガーの画像を表示しまーす。
	
	sprite[number]->SetPosition(m_slidePos[number]);
}