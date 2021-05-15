#include "stdafx.h"
#include "MissCounter.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "Result.h"

bool MissCounter::Start()
{
	m_level2D.Init("Assets/level2D/level2D_add.casl", [&](Level2DObjectData& objdata) {
		if (strncmp(objdata.name, "square", 6) == 0 ) {
			int num = atoi(&objdata.name[6]);

			m_spriteRender[num - 1] = NewGO<SpriteRender>(0);
			m_spriteRender[num - 1]->Init("Assets/Image/square.dds", 24, 24);
			m_spriteRender[num - 1]->SetPosition(objdata.position);

			return true;
		}
		else {
			//return falseにすると、
			//Level2DクラスのSpriteで画像が読み込まれます。
			return true;
		}
		});

	return true;
}

void MissCounter::Update()
{
	if (shouldChangeFilePath) {
		switch (pl2MissCount) {
		case 1:
			m_spriteRender[0]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 2:
			m_spriteRender[1]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 3:
			m_spriteRender[2]->Init("Assets/Image/squareCross.dds", 24, 24);
			pl1Win = true;
			//2P側に勝利表示を付ける
			//if (m_result == nullptr) {
			//	m_result = NewGO<Result>(0);
			//	m_result->SetSprite(1);
			//	m_result->SetSpritePos(1); //1 : 右側
			//	m_result->Reach3Miss(true);
			//}
			break;
		}

		switch (pl1MissCount + 3) {
		case 4:
			m_spriteRender[3]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 5:
			m_spriteRender[4]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 6:
			m_spriteRender[5]->Init("Assets/Image/squareCross.dds", 24, 24);
			pl2Win = true;
			//if (m_result == nullptr) {
			//	m_result = NewGO<Result>(0);
			//	m_result->SetSprite(1);
			//	m_result->SetSpritePos(2); //1 : 右側
			//	m_result->Reach3Miss(true);
			//}
			break;
		}
		shouldChangeFilePath = false;

		//同時に負けたとき
		if (pl1Win == true && pl2Win == true) {
			m_result = NewGO<Result>(0);
			m_result->SetSprite(0);					//Draw
			m_result->SetSpritePos(0);				//1 : 右側
			m_result->Reach3Miss(true);
		}
		//1PWIN
		else if (pl1Win) {
			m_result = NewGO<Result>(0);
			m_result->SetSprite(1);					//Win
			m_result->SetSpritePos(1);				//1 : 右側
			m_result->Reach3Miss(true);
		}
		//2PWIN
		else if (pl2Win) {
			m_result = NewGO<Result>(0);
			m_result->SetSprite(1);					//Win
			m_result->SetSpritePos(2);				//Left
			m_result->Reach3Miss(true);
		}
	}
}