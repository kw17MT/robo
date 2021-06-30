#include "stdafx.h"
#include "Player.h"
#include "Guzai.h"
#include "FontRender.h"
#include "PathFactory.h"
#include "GameDirector.h"
#include "SkinModelRender.h"
#include "Kitchen.h"

namespace
{
	const Vector3 EFFECT_SCALE = { 10.0f,10.0f,10.0f };
	const Vector3 FRONT_DIRECTION = { 0.0f,0.0f,1.0f };
	const Vector3 RIGHT_DIRECTION = { 1.0f,0.0f,0.0f };
	const Vector3 PLYAER_SPEED_ZERO = Vector3::Zero;

	const int KITCHEN_NAME_SIZE = 10;
	const int PLAYER_NUMBER_ONE = 0;
	const int PLAYER_NUMBER_TWO = 1;
	const int MAX_NUM_TO_GUZAI_STACK = 10;
	const int STACK_NONE = 9;
	const int PLAYER_1_RESTRECTED_POS_X_MIN = 900;
	const int PLAYER_2_RESTRECTED_POS_X_MIN = -1300;
	const int PLAYER_1_RESTRECTED_POS_X_MAX = 1300;
	const int PLAYER_2_RESTRECTED_POS_X_MAX = -900;
	const int PLAYER_RESTRECTED_POS_Z_MIN = -530;
	const int PLAYER_RESTRECTED_POS_Z_MAX = 190;
	const int EFFECT_POP_RATE = 12;
	const int EFFECT_POP = 11;
	const int EFFECT_TIMER_MAX = 60;

	const float AJUST_PLAYER_SPEED = -10.0f;
}

Player::~Player()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_effect);
}

bool Player::Start()
{
	//通常モデル
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//影用モデル
	if (m_playerNo == PLAYER_NUMBER_ONE) {
		//通常描画
		m_skinModelRender->Init(
			"Assets/modelData/Chef/ChefRed/Chef01.tkm",
			"Assets/modelData/Chef/ChefRed/Chef_1.tks",
			enModelUpAxisZ,
			m_position
		);
		//影描画用
		m_skinModelRender->InitForCastShadow(
			"Assets/modelData/Chef/ChefRed/Chef01.tkm",
			"Assets/modelData/Chef/ChefRed/Chef_1.tks",
			enModelUpAxisZ,
			m_position,
			nullptr
		);

		
	}
	else {
		m_skinModelRender->Init(
			"Assets/modelData/Chef/ChefBlue/Chef02.tkm",
			"Assets/modelData/Chef/ChefRed/Chef_1.tks",
			enModelUpAxisZ,
			m_position
		);
		m_skinModelRender->InitForCastShadow(
			"Assets/modelData/Chef/ChefBlue/Chef02.tkm",
			"Assets/modelData/Chef/ChefRed/Chef_1.tks",
			enModelUpAxisZ,
			m_position,
			nullptr
		);
	}

	m_skinModelRender->SetScale(m_scale);

	//具材ナンバー配列のすべての要素を9で初期化
	for (int i = 0; i < MAX_NUM_TO_GUZAI_STACK; i++) {
		m_guzaiNo[i] = STACK_NONE;
	}

	int endNo = m_playerNo;
	//string型に変えてcharに変換するための準備をする。
	std::string endNo_string = std::to_string(endNo);
	//不変箇所を定義
	char kitchenName[KITCHEN_NAME_SIZE] = "kitchen0";
	//末端番号だけを追加する
	strcat_s(kitchenName, endNo_string.c_str());
	//所定のキッチンを設定後、座標をとってきてセットする。
	Kitchen* kitchen = FindGO<Kitchen>(kitchenName);

	//エフェクトの初期化
	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/dust.efk");
	m_effect->SetScale(EFFECT_SCALE);

	return true;
}

void Player::SetGuzaiNo9()
{
	for (int i = 0;i < MAX_NUM_TO_GUZAI_STACK;i++)
	{
		m_guzaiNo[i] = STACK_NONE;
	}
}

void Player::RestrictPos()
{
	//座標を用いてプレイヤーの移動範囲を管理する。
	if (m_playerNo == PLAYER_NUMBER_ONE) {
		if (m_position.x > PLAYER_1_RESTRECTED_POS_X_MAX) {
			m_position.x = PLAYER_1_RESTRECTED_POS_X_MAX;
		}
		if (m_position.x < PLAYER_1_RESTRECTED_POS_X_MIN) {
			m_position.x = PLAYER_1_RESTRECTED_POS_X_MIN;
		}
		if (m_position.z > PLAYER_RESTRECTED_POS_Z_MAX) {
			m_position.z = PLAYER_RESTRECTED_POS_Z_MAX;
		}
		if (m_position.z < PLAYER_RESTRECTED_POS_Z_MIN) {
			m_position.z = PLAYER_RESTRECTED_POS_Z_MIN;
		}
	}
	if (m_playerNo == PLAYER_NUMBER_TWO) {
		if (m_position.x > PLAYER_2_RESTRECTED_POS_X_MAX) {
			m_position.x = PLAYER_2_RESTRECTED_POS_X_MAX;
		}
		if (m_position.x < PLAYER_2_RESTRECTED_POS_X_MIN) {
			m_position.x = PLAYER_2_RESTRECTED_POS_X_MIN;
		}
		if (m_position.z > PLAYER_RESTRECTED_POS_Z_MAX) {
			m_position.z = PLAYER_RESTRECTED_POS_Z_MAX;
		}
		if (m_position.z < PLAYER_RESTRECTED_POS_Z_MIN) {
			m_position.z = PLAYER_RESTRECTED_POS_Z_MIN;
		}
	}
}

void Player::Update()
{
	//カウントダウン中でも、プレイヤーの初期位置は固定しておきたいため。
	m_skinModelRender->SetPosition(m_position);

	//ゲームプレイ中じゃなかったら。
	if (!GetGameDirector().GetIsGamePlay())
	{
		//処理しない。
		return;
	}

	//回転処理
	//自身の上と右を定義(見下ろしなので)
	Vector3 frontDirP1 = FRONT_DIRECTION;
	frontDirP1.Normalize();
	Vector3 rightDirP1 = RIGHT_DIRECTION;
	rightDirP1.Normalize();

	//回転軸の決定
	Vector3 AxisYP1;
	AxisYP1.Cross(frontDirP1, rightDirP1);

	//水平方向と奥行方向への入力を受け取り
	float LStickXP1 = g_pad[m_playerNo]->GetLStickXF();
	float LStickZP1 = g_pad[m_playerNo]->GetLStickYF() * -1.0f; //奥行方向の逆転を-1.0fを掛けて補正

	if (fabsf(LStickXP1) < 0.001f && fabsf(LStickZP1) < 0.001f) {
		//return; //returnすると以下の処理がすっ飛ばされてUpdateの最後にいってしまう。
	}
	else {
		//二つの入力値が成す角を求める(ラジアン角)
		m_angle = atan2(LStickXP1, LStickZP1);
		//縦軸まわりの回転を求める(ラジアン角を引数に渡すためSetRotation)
		m_rotation.SetRotation(AxisYP1, -m_angle);
	}


	if (m_moveStop == false) {
		m_moveSpeed.x = g_pad[m_playerNo]->GetLStickXF() * AJUST_PLAYER_SPEED;
		m_moveSpeed.z = g_pad[m_playerNo]->GetLStickYF() * AJUST_PLAYER_SPEED;
	}
	if (m_moveStop == true)
	{
		m_moveSpeed = PLYAER_SPEED_ZERO;
	}

	m_position += m_moveSpeed;

	//プレイヤーが移動している限り移動速度を保存し続ける。
	if (m_moveSpeed.x != 0.0f || m_moveSpeed.z != 0.0f) {
		m_saveMoveSpeed = m_moveSpeed;
		m_saveMoveSpeed.Normalize();
	}

	RestrictPos();

	m_skinModelRender->SetPosition(m_position);

	//エフェクト再生
	//移動中なら定期的に発生
	m_moveCounter++;
	if (m_moveSpeed.x != 0) {
		if (m_moveCounter % EFFECT_POP_RATE == EFFECT_POP) {
			m_effect->SetPosition(m_position);
			m_effect->Play(0);
		}
	}

	if (m_moveCounter > EFFECT_TIMER_MAX) {
		m_moveCounter = 0;
	}

	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRotation(m_rotation);
	m_effect->Update();
}