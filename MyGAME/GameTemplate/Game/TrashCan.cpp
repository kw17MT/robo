#include "stdafx.h"
#include "TrashCan.h"
#include "SkinModelRender.h"
#include "Player.h"
#include "PlayerGene.h"

namespace
{
	const int PLAYER_NAME_SIZE = 9;
	const float AJUST_ARROW_POS_Y = 300.0f;
	const int TRASHCAN_SHRINK_TIME_MAX = 30;
	const int TRASHCAN_SHRINK_TIME_MIN = 20;
	const int TRASHCAN_EXPAND_TIME_MAX = 20;
	const int TRASHCAN_EXPAND_TIME_MIN = 0;
	const float TRASHCAN_SHRINK_RATE = 0.03f;
	const float TRASHCAN_EXPAND_RATE = 0.015f;
	const int DEFAULT_MOVING_TIME = 30;
	const float ARROW_CHANGE_SCALE_RATE = 0.1f;
	const float ARROW_MAX_SCALE = 1.0f;
	const float ARROW_MIN_SCLAE = 0.0f;
	const float ARROW_MAX_HEIGHT = 175.0f;
	const float ARROW_MIN_HEIGHT = 150.0f;
	const float ARROW_MOVE_RATE = 1.0f;
}

TrashCan::~TrashCan()
{
	//ゴミ箱を削除
	DeleteGO(m_skinModelRender);
	//矢印を削除
	DeleteGO(m_arrow);
}

bool TrashCan::Start()
{
	//string型に変えてcharに変換するための準備をする。
	std::string endNo_string = std::to_string(m_trashcanNo);
	//不変箇所を定義
	char playerName[PLAYER_NAME_SIZE] = "player0";
	//末端番号だけを追加する
	strcat_s(playerName, endNo_string.c_str());
	//必要なデータの探索と確保
	m_player = FindGO<Player>(playerName);
	m_playerGene = FindGO<PlayerGene>("playerGene");

	//ゴミ箱モデルの設定
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//通常描画用モデルの初期化
	m_skinModelRender->InitForRecieveShadow("Assets/modelData/trashcan/trashcan.tkm", nullptr, enModelUpAxisZ, m_position);
	//影生成用の初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/trashcan/trashcan.tkm", nullptr, enModelUpAxisZ, m_position);
	//拡大率を調整
	m_skinModelRender->SetScale(m_trashcanScale);

	//ゴミ箱に近づくと矢印が出るように
	m_arrow = NewGO<SkinModelRender>(0);
	//通常モデルの初期化
	m_arrow->Init("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_position);
	//シャドウキャスト用の初期化
	m_arrow->InitForCastShadow("Assets/modelData/Arrow/Arrow_Yellow.tkm", nullptr, enModelUpAxisZ, m_position);
	//拡大率を調整
	m_arrow->SetScale(m_targetScale);

	//矢印の位置をゴミ箱の上に設定しておく
	m_arrowPos = m_position;
	//ゴミ箱の少し上に調整
	m_arrowPos.y +=AJUST_ARROW_POS_Y;
	//矢印の位置を更新
	m_arrow->SetPosition(m_arrowPos);
	m_arrow->SetRotation(m_rot);

	return true;
}

void TrashCan::LetStartMoving()
{
	//ゴミ箱が動いているとき
	if (m_isMoving) {
		m_movingTime--;																					
		//Y座標における拡大率を減少させてへこませる														//初期値30
		if (m_movingTime > TRASHCAN_SHRINK_TIME_MIN && m_movingTime <= TRASHCAN_SHRINK_TIME_MAX) {		//30~21　10フレーム
			m_trashcanScale.y -= TRASHCAN_SHRINK_RATE;													//10 * -0.03f = -0.3f
		}
		if (m_movingTime > TRASHCAN_EXPAND_TIME_MIN && m_movingTime <= TRASHCAN_EXPAND_TIME_MAX) {		//20~1　20フレーム
			m_trashcanScale.y += TRASHCAN_EXPAND_RATE;													//20 * 0.015f = 0.3f
		}
		if (m_movingTime <= 0) {
			m_movingTime = DEFAULT_MOVING_TIME;
			m_isMoving = false;
		}
	}
}

void TrashCan::ChangeArrowScale()
{
	//プレイヤーとの距離を測る。
	float playerDistance = CalcDistance(m_player->GetPosition(), m_position);

	//矢印の大きさ変更用。近づくと大きくなり、離れると小さくなる。
	if (playerDistance < m_distance) {
		m_targetScale.x += ARROW_CHANGE_SCALE_RATE;
		m_targetScale.y += ARROW_CHANGE_SCALE_RATE;
		m_targetScale.z += ARROW_CHANGE_SCALE_RATE;

		if (m_targetScale.x >= ARROW_MAX_SCALE) {
			m_targetScale.x = ARROW_MAX_SCALE;
			m_targetScale.y = ARROW_MAX_SCALE;
			m_targetScale.z = ARROW_MAX_SCALE;
		}
		m_canTrash = true;
	}
	else {
		m_targetScale.x -= ARROW_CHANGE_SCALE_RATE;
		m_targetScale.y -= ARROW_CHANGE_SCALE_RATE;
		m_targetScale.z -= ARROW_CHANGE_SCALE_RATE;

		if (m_targetScale.x <= ARROW_MIN_SCLAE) {
			m_targetScale.x = ARROW_MIN_SCLAE;
			m_targetScale.y = ARROW_MIN_SCLAE;
			m_targetScale.z = ARROW_MIN_SCLAE;
		}
		m_canTrash = false;
	}
}

void TrashCan::FloatArrow()
{
	//矢印の浮遊処理
	if (m_targetUp == true) {
		m_arrowPos.y += ARROW_MOVE_RATE;
		if (m_arrowPos.y >= ARROW_MAX_HEIGHT) {
			m_targetUp = false;
		}
	}

	if (m_targetUp == false) {
		m_arrowPos.y -= ARROW_MOVE_RATE;
		if (m_arrowPos.y <= ARROW_MIN_HEIGHT) {
			m_targetUp = true;
		}
	}
}

void TrashCan::Update()
{
	//プレイヤーが完全に出現したらIF文以降の処理を行う
	if (m_playerGene->GetPlayerGeneState() == true) {
		return;
	}

	//プレイヤーの情報が確定しないままの時があるため、最終確認
	if (m_player == nullptr) {
		//string型に変えてcharに変換するための準備をする。
		std::string endNo_string = std::to_string(m_trashcanNo);
		//不変箇所を定義
		char playerName[PLAYER_NAME_SIZE] = "player0";
		//末端番号だけを追加する
		strcat_s(playerName, endNo_string.c_str());
		//必要なデータの探索と確保
		m_player = FindGO<Player>(playerName);
	}

	//プレイヤーとの距離に応じて矢印の拡大率を変化させる。
	ChangeArrowScale();

	//矢印の浮遊
	FloatArrow();

	//ゴミ箱に何か入れられたときにリアクションをする。
	LetStartMoving();

	m_arrow->SetPosition(m_arrowPos);
	m_arrow->SetScale(m_targetScale);
	m_skinModelRender->SetPosition(m_position);
	m_skinModelRender->SetScale(m_trashcanScale);
}