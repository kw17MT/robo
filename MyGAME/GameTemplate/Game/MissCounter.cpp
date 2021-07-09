#include "stdafx.h"
#include "MissCounter.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "Result.h"

namespace
{
	const int PHOTO_SIZE_WIDTH = 36;
	const int PHOTO_SIZE_HEIGHT = 36;
	const int PHOTO_NAME_SIZE = 6;
	const int PHOTO_NAME_NUMBER = 6;
	const int AJUST_ARRAY_NUMBER = 1;
	const int PLAYER_1_FAR_RIGHT = 3;
	const int PLAYER_1_MIDDLE = 4;
	const int PLAYER_1_FAR_LEFT = 5;
	const int PLAYER_2_FAR_RIGHT = 0;
	const int PLAYER_2_MIDDLE = 1;
	const int PLAYER_2_FAR_LEFT = 2;
	const int PLAYER_1_MISS_ONE = 4;
	const int PLAYER_1_MISS_TWO = 5;
	const int PLAYER_1_MISS_THREE = 6;
	const int PLAYER_2_MISS_ONE = 1;
	const int PLAYER_2_MISS_TWO = 2;
	const int PLAYER_2_MISS_THREE = 3;
	const int PLAYER_BOTH_SIDE = 0;
	const int PLAYER_1_SIDE = 1;
	const int PLAYER_2_SIDE = 2;
	const int DRAW_DRAW = 0;
	const int DRAW_WIN = 1;
	const int DRAW_LOSE = 2;
	const int PLAYER_1_RESULT = 0;
	const int PLAYER_2_RESULT = 1;
	const int RESULT_DRAW = 0;
	const int AJUST_ARRAY_MISS_NUMBER = 3;
}

MissCounter::~MissCounter()
{
	//ミスカウント画像を消去
	for (int i = 0; i < 6; i++) {
		DeleteGO(m_spriteRender[i]);
	}
	//リザルト画像が出ていたら消去
	for (int i = 0; i < 2; i++) {
		if (m_result[i] != nullptr) {
			DeleteGO(m_result[i]);
		}
	}
}

bool MissCounter::Start()
{
	//レベルを用いた画像の表示
	m_level2D.Init("Assets/level2D/level2D_add.casl", [&](Level2DObjectData& objdata) {
		//SQUAREがついている画像があった場合
		if (strncmp(objdata.name, "square", PHOTO_NAME_SIZE) == 0 ) {
			//その画像のナンバリングを取得
			int num = atoi(&objdata.name[PHOTO_NAME_NUMBER]);
			//ナンバリングが１からなので、配列先頭から格納するために数値を調整
			m_spriteRender[num - AJUST_ARRAY_NUMBER] = NewGO<SpriteRender>(0);
			//画像の初期化
			m_spriteRender[num - AJUST_ARRAY_NUMBER]->Init("Assets/Image/square.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			//この画像をレベルの位置の通りに置く
			m_spriteRender[num - AJUST_ARRAY_NUMBER]->SetPosition(objdata.position);

			return true;
		}
		else {
			return true;
		}
		});

	return true;
}

void MissCounter::TurnSquareIntoCross()
{
	//どれかの画像にバツを付け加えるべきか。
	if (m_shouldChangeFilePath) {
		//ミスの数に応じてどこにバツを付けるか決める
		//プレイヤー１の方の判定を行う。ミス数と配列の要素番号を合致させるため数値を調節
		switch (m_pl1MissCount + AJUST_ARRAY_MISS_NUMBER) {
			//ミスを１回していたらバツが画像に変える
		case PLAYER_1_MISS_ONE:
			m_spriteRender[PLAYER_1_FAR_RIGHT]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			break;
			//ミスを２回していたらバツが画像に変える
		case PLAYER_1_MISS_TWO:
			m_spriteRender[PLAYER_1_MIDDLE]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			break;
			//ミスを３回していたらバツが画像に変える
		case PLAYER_1_MISS_THREE:
			m_spriteRender[PLAYER_1_FAR_LEFT]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			//２Pが勝ったことを知らせる
			m_pl2Win = true;
			break;
		}
		//プレイヤー２の方の判定を行う
		switch (m_pl2MissCount) {
		case PLAYER_2_MISS_ONE:
			m_spriteRender[PLAYER_2_FAR_RIGHT]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			break;
		case PLAYER_2_MISS_TWO:
			m_spriteRender[PLAYER_2_MIDDLE]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			break;
		case PLAYER_2_MISS_THREE:
			m_spriteRender[PLAYER_2_FAR_LEFT]->Init("Assets/Image/squareCross.dds", PHOTO_SIZE_WIDTH, PHOTO_SIZE_HEIGHT);
			m_pl1Win = true;
			break;
		}

		//付け終わたのでfalseに戻す
		m_shouldChangeFilePath = false;
	}
}

void MissCounter::PopResultSprite()
{
	//同時に勝った
	if (m_pl1Win == true && m_pl2Win == true) {
		m_result[RESULT_DRAW] = NewGO<Result>(0);
		m_result[RESULT_DRAW]->SetSprite(DRAW_DRAW);					//Drawの画像を表示
		m_result[RESULT_DRAW]->SetSpritePos(PLAYER_BOTH_SIDE);			//中央に描く
		m_result[RESULT_DRAW]->Reach3Miss(true);
		m_isGameSet = true;
	}
	//1PWIN
	else if (m_pl1Win) {
		m_result[PLAYER_1_RESULT] = NewGO<Result>(0);
		m_result[PLAYER_1_RESULT]->SetSprite(DRAW_WIN);					//Winの画像を描く
		m_result[PLAYER_1_RESULT]->SetSpritePos(PLAYER_1_SIDE);			//左側に描く
		m_result[PLAYER_1_RESULT]->Reach3Miss(true);
		m_result[PLAYER_2_RESULT] = NewGO<Result>(0);
		m_result[PLAYER_2_RESULT]->SetSprite(DRAW_LOSE);				//Loseの画像を描く
		m_result[PLAYER_2_RESULT]->SetSpritePos(PLAYER_2_SIDE);			//右側に描く
		m_result[PLAYER_2_RESULT]->Reach3Miss(true);

		m_isGameSet = true;
	}
	//2PWIN
	else if (m_pl2Win) {
		m_result[PLAYER_1_RESULT] = NewGO<Result>(0);
		m_result[PLAYER_1_RESULT]->SetSprite(DRAW_LOSE);				//Loseの画像を描く
		m_result[PLAYER_1_RESULT]->SetSpritePos(PLAYER_1_SIDE);			//左側に描く
		m_result[PLAYER_1_RESULT]->Reach3Miss(true);
		m_result[PLAYER_2_RESULT] = NewGO<Result>(0);
		m_result[PLAYER_2_RESULT]->SetSprite(DRAW_WIN);					//Winの画像を描く
		m_result[PLAYER_2_RESULT]->SetSpritePos(PLAYER_2_SIDE);			//右側に描く
		m_result[PLAYER_2_RESULT]->Reach3Miss(true);
		m_isGameSet = true;
	}
}

void MissCounter::Update()
{
	//ゲームが終わっていたら
	if (m_isGameSet == true) {
		return;
	}
	
	//四角画像をバツに変えるべきかどうか判定する
	TurnSquareIntoCross();

	//結果画像を出現させるかどうかの判定を行う。
	PopResultSprite();
}