#include "stdafx.h"
#include "MissCounter.h"
#include "SpriteRender.h"
#include "Level2D.h"

bool MissCounter::Start()
{
	m_level2D.Init("Assets/level2D/level2D_new.casl", [&](Level2DObjectData& objdata) {
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
		switch (pl1MissCount) {
		case 1:
			m_spriteRender[0]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 2:
			m_spriteRender[1]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 3:
			m_spriteRender[2]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		}

		switch (pl2MissCount + 3) {
		case 4:
			m_spriteRender[3]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 5:
			m_spriteRender[4]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		case 6:
			m_spriteRender[5]->Init("Assets/Image/squareCross.dds", 24, 24);
			break;
		}
		shouldChangeFilePath = false;
	}

}