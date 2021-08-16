#include "stdafx.h"
#include "Dish.h"
#include "SkinModelRender.h"
#include "Level.h"
#include "Guzai.h"
#include "PlayerGene.h"
#include "PathMove.h"
#include "SoundSource.h"
#include "DishGene.h"
#include "GuzaiManager.h"
#include "DishManager.h"

namespace
{
	const float MOVE_SPEED = 70.0f;
	const float DROP_POS_PHASE1 = 800.0f;
	const float DROP_SPEED_PHASE1 = 4.0f;
	const float DROP_POS_PHASE2 = 10.0f;
	const float DROP_SPEED_PHASE2 = 20.0f;
	const float DROP_POS_TOP = 1000.0f;
	const float SE_FALLING_VOLUME = 0.3f;
	const float SE_VOLUME = 0.1f;
	const float SE_FIRST_POP_VOLUME = 0.02f;
	const int GUZAI_POP_DELAY = 60;
}

Dish::~Dish()
{
	//この皿が出した具材をすべて削除のつもり
	for (Guzai* guzai : m_guzai)
	{
		if (guzai != nullptr) {
			DeleteGO(guzai);
		}
	}
	//全ての具材の要素を全削除
	m_guzai.clear();

	//皿のモデルを削除
	DeleteGO(m_skinModelRender);
}

bool Dish::Start()
{
	//皿のモデル初期化
	m_skinModelRender = NewGO<SkinModelRender>(0,"dish");
	//シャドウレシーバーのオブジェクトを初期化
	m_skinModelRender->Init/*ForRecieveShadow*/("Assets/modelData/dish/dish.tkm", nullptr, enModelUpAxisZ, m_position);
	//シャドウキャスターのオブジェクトを初期化
	m_skinModelRender->InitForCastShadow("Assets/modelData/dish/dish.tkm", nullptr, enModelUpAxisZ, m_position);
	//プレイヤー生成器のオブジェクトを取得
	m_playerGene = FindGO<PlayerGene>("playerGene");
	//皿生成器のオブジェクトを取得
	m_dishGene = FindGO<DishGene>("dishGene");

	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_position, MOVE_SPEED, enNormalLane);

	//拡大率を設定
	m_skinModelRender->SetScale(m_scale);

	return true;
}

void Dish::Move()
{
	//皿をウェイポイントで移動させる。
	m_position = m_pathMove.get()->Move();
	//位置を更新
	m_skinModelRender->SetPosition(m_position);
	//最後のポイントまで到達したら。
	if (m_pathMove.get()->GetIsFinalPoint())
	{
	}
}

void Dish::PopGuzaiOnDish()
{
	//この皿は具材を持っているなら
	//具材のポップ時に呼ばれる
	if (m_isHavingGuzai == true) {
		if (m_popedSound)
		{
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/thi-n.wav", false);
			se->SetVolume(SE_VOLUME);
			se->Play(false);
			//ポップした瞬間の1回だけ音を鳴らせばいいのでここでFALSEに
			m_popedSound = false;
		}
		//具材の位置を皿の上に
		m_guzaiPos = m_position;
		//Y軸の位置だけに加算
		m_guzaiPos.y += m_guzaiYPos;
		//自分の上に出現したことのある具材で最後に出てきたやつの位置を更新
		m_guzai.back()->SetPosition(m_guzaiPos);
		//再ポップが行われて、具材が皿のかなり高い位置にあるとき
		if (m_guzaiYPos > DROP_POS_PHASE1) {
			//ゆっくり落ちてくる。
			m_guzaiYPos -= DROP_SPEED_PHASE1;
		}
		//一定以上落ちてきたら
		else if (m_guzaiYPos > DROP_POS_PHASE2) {
			//素早く落ちてくる
			m_guzaiYPos -= DROP_SPEED_PHASE2;
			if (m_dishGene->GetFallingSound() == false) {
				//落下音を鳴らす
				CSoundSource* se = NewGO<CSoundSource>(0);
				se->Init(L"Assets/sound/falling2.wav", false);
				se->SetVolume(SE_FALLING_VOLUME);
				se->Play(false);
				m_dishGene->SetFallingSound(true);
			}
		}
	}
	else {
		//次ポップしてくる具材のためにデフォルト値で初期化
		m_guzaiYPos = DROP_POS_TOP;
	}
}

void Dish::ShouldRefill()
{
	//補充するように命令されたか
	if (GuzaiManager::GetInstance().IsOrderedRefill()) {
		//発生するまでの遅延を発生させる
		m_guzaiTimer++;
		if (m_guzaiTimer > GUZAI_POP_DELAY) {
			//この皿は具材を持っていなかったら
			if (m_isHavingGuzai == false) {
				//追加の具材を出す。
				m_guzai.push_back(NewGO<Guzai>(0));
				m_isHavingGuzai = true;
				m_popedSound = true;
				//補充した皿の枚数を１足す
				GuzaiManager::GetInstance().AddRefilledGuzaiNum();
			}
			//補充した皿の数が空だった皿の数と同じになったら、０で初期化
			if (GuzaiManager::GetInstance().GetRefilledGuzaiNum() >= GuzaiManager::GetInstance().GetEmptyDishNum()) {
				//空の皿、補充した回数を０にリセットする。
				GuzaiManager::GetInstance().ResetParamAboutDishAndRefill();
				//命令を「何もしない」にしておく
				GuzaiManager::GetInstance().SetNothingOrder();
				m_dishGene->SetFallingSound(false);
			}
			m_guzaiTimer = 0;
		}
	}
}

void Dish::Update()
{
	//最初だけすべての皿の上に出す。
	if (m_isCompletedFirstPop == false) {
		//プレイヤーは具材より先にしっかりと出ているか
		if (m_playerGene->GetPlayerGeneState() == false) {
			//一つ目の具材を出す。
			m_guzai.push_back(NewGO<Guzai>(0));
			//この皿は具材持っている。
			m_isHavingGuzai = true;
			//最初のポップが完了した。
			m_isCompletedFirstPop = true;

			//最初に音をならす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/thi-n.wav", false);
			se->SetVolume(SE_FIRST_POP_VOLUME);
			se->Play(false);
		}
	}
	
	//自分の上の具材が持たれているならば
	if (m_guzai.back()->GetisHadState() == true) {
		m_isHavingGuzai = false;
	}

	//補充の命令が来ていたら自身の上に具材を補充
	ShouldRefill();

	//皿の移動速度アップの命令が来ていたら
	if (DishManager::GetInstance().GetSpeedUpState() == true) {
		//移動速度を2倍にする。
		float moveSpeed = MOVE_SPEED * 2.0f;
		m_pathMove.get()->ChangeMoveSpeed(moveSpeed);
	}
	else{
		//命令が終わったらor来ていなければ通常速度にする。
		m_pathMove.get()->ChangeMoveSpeed(MOVE_SPEED);
	}

	//移動処理
	Move();

	//具材を皿の上に出す。
	PopGuzaiOnDish();

	//拡大率の更新
	m_skinModelRender->SetScale(m_scale);
}