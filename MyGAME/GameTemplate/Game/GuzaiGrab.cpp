#include "stdafx.h"
#include "GuzaiGrab.h"
#include "Player.h"
#include "SoundSource.h"
#include "GuzaiManager.h"

//void GuzaiGrab::Grab(int playerNumber, Vector3 playerPos, int& playerState, bool& isTargeted, bool& isPutOnKitchen, bool& isPlayerCooking, int& whichPlayerGet, int& isHad)
//{
//	if (g_pad[playerNumber]->IsTrigger(enButtonA)) {
//		//私（この具材）はプレイヤー１にロックオンされている。
//			//プレイヤー1は何も持っていない。私はターゲットされている（距離測定済）。自身はキッチン上にない。プレイヤー1はバーガー作成中ではない。
//			//最後の引数は、キッチン上でハンバーガーを作るために一か所に集まっている最中に取れないようにするため。
//			if (playerState == 0 && isTargeted == true && isPutOnKitchen == false && isPlayerCooking == false) {
//				//もたれた！
//				isHad = true;
//				//Player0は具材をもっている！
//				//m_player00->SetPlayerState(enHaveGuzai);
//				playerState = 1;
//				//自分はどっちのプレイヤーに持たれたか
//				whichPlayerGet = playerNumber;
//				//音を鳴らす
//				CSoundSource* se = NewGO<CSoundSource>(0);
//				se->Init(L"Assets/sound/poka01.wav", false);
//				se->SetVolume(1.0f);
//				se->Play(false);
//				//普通に流れている具材をとった時
//				if (m_guzaiOkibaSet == false) {
//					//素直に空の皿の数を1増やす
//					GuzaiManager::GetInstance().AddEmptyDishNum();
//				}
//				//補充を開始するかどうか判断する。
//				GuzaiManager::GetInstance().JudgeToOrderRefill();
//				//それが具材置き場にあった時の処理
//				//AwayFromGuzaiOkiba();
//			}
//	}
//}