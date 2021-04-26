#include "stdafx.h"
#include "PlayerGene.h"
#include "Player.h"

bool PlayerGene::Start()
{
	return true;
}

void PlayerGene::Update()
{
	if (isWorking) {
		if (playerNum == 0) {
			player[0] = NewGO<Player>(0, "player01");
			player[0]->SetPosition(player01Pos);
			player01Rot.SetRotationDegY(90.0f);
			player[0]->SetRotation(player01Rot);
			player[0]->SetPlayerNo(1);
			playerNum++;
		}
		if (playerNum == 1) {
			player[1] = NewGO<Player>(0, "player02");
			player[1]->SetPosition(player02Pos);
			player02Rot.SetRotationDegY(270.0f);
			player[1]->SetRotation(player02Rot);
			player[1]->SetPlayerNo(2);
			playerNum++;
			isWorking = false;
		}
	}
}