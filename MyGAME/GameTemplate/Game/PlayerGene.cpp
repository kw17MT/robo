#include "stdafx.h"
#include "PlayerGene.h"
#include "Player.h"

namespace
{
	const int PLAYER_NUMBER_ONE = 0;
	const int PLAYER_NUMBER_TWO = 1;
	const int PLAYER_MAX_NUM = 2;
}

PlayerGene::~PlayerGene()
{
	//プレイヤーの削除
	for (int i = 0; i < PLAYER_MAX_NUM; i++) {
		if (m_player[i] != nullptr) {
			DeleteGO(m_player[i]);
		}
	}
}

void PlayerGene::Update()
{
	//プレイヤーの生成器が動いているとき
	if (m_isWorking) {
		//プレイヤー１の生成
		m_player[PLAYER_NUMBER_ONE] = NewGO<Player>(0, "player00");
		//プレイヤー１の座標設定
		m_player[PLAYER_NUMBER_ONE]->SetPosition(m_player01Pos);
		//一番初めのプレイヤーの向きを決定
		m_player01Rot.SetRotationDegY(90.0f);
		m_player[PLAYER_NUMBER_ONE]->SetRotation(m_player01Rot);
		//プレイヤー番号を設定してやる
		m_player[PLAYER_NUMBER_ONE]->SetPlayerNo(PLAYER_NUMBER_ONE);

		//プレイヤー２の生成
		m_player[PLAYER_NUMBER_TWO] = NewGO<Player>(0, "player01");
		m_player[PLAYER_NUMBER_TWO]->SetPosition(m_player02Pos);
		m_player02Rot.SetRotationDegY(270.0f);
		m_player[PLAYER_NUMBER_TWO]->SetRotation(m_player02Rot);
		m_player[PLAYER_NUMBER_TWO]->SetPlayerNo(PLAYER_NUMBER_TWO);

		//この生成器の動きを止める
		m_isWorking = false;
	}
}