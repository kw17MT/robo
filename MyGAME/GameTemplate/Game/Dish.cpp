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
	const float LIGHT_DIRECTION_PISITIVE = 1.0f;
	const Vector3 LIGHT_DIRECTION_RIGHTER = { 100.0f,100.0f,100.0f };
	const float DROP_POS_PHASE1 = 800.0f;
	const float DROP_SPEED_PHASE1 = 4.0f;
	const float DROP_POS_PHASE2 = 10.0f;
	const float DROP_SPEED_PHASE2 = 20.0f;
	const float DROP_POS_TOP = 1000.0f;
	const float SE_VOLUME = 0.5f;
	const Vector3 AJUST_AMBIENTLIGHT = { 0.98f,0.98f,0.98f };
	const int GUZAI_POP_DELAY = 60;
	const int MAX_GUZAI_POP_NUM = 99;
}

Dish::~Dish()
{
	//皿のモデルを削除
	DeleteGO(m_skinModelRender);
	
	//この皿が出した具材をすべて削除のつもり
	for (Guzai* guzai : m_guzai)
	{
		DeleteGO(guzai);
	}
	//全ての具材の要素を全削除
	m_guzai.clear();
}

bool Dish::Start()
{
	//皿、特有のライトを設定する。
	m_lig.ambientLight.Set(AJUST_AMBIENTLIGHT);
	m_lig.directionalLight.direction.x = LIGHT_DIRECTION_PISITIVE;
	m_lig.directionalLight.direction.y = LIGHT_DIRECTION_PISITIVE;
	m_lig.directionalLight.direction.z = LIGHT_DIRECTION_PISITIVE;
	m_lig.directionalLight.color.Set(LIGHT_DIRECTION_RIGHTER);
	m_lig.directionalLight.direction.Normalize();

	//皿のモデル初期化
	m_skinModelRender = NewGO<SkinModelRender>(0,"dish");
	m_skinModelRender->Init("Assets/modelData/object/dish.tkm", nullptr, enModelUpAxisZ, m_position, &m_lig);
	m_skinModelRender->SetScale(m_scale);
	//m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);
	m_skinModelRender->InitForCastShadow("Assets/modelData/object/dish.tkm", nullptr, enModelUpAxisZ, m_position, &m_lig);
	m_playerGene = FindGO<PlayerGene>("playerGene");
	m_dishGene = FindGO<DishGene>("dishGene");

	m_pathMove = std::make_unique<PathMove>();
	m_pathMove.get()->Init(m_position, MOVE_SPEED, enNormalLane);

	return true;
}

void Dish::Move()
{
	//皿をウェイポイントで移動させる。
	m_position = m_pathMove.get()->Move();
	m_skinModelRender->SetPosition(m_position);
	//最後のポイントまで到達したら。
	if (m_pathMove.get()->GetIsFinalPoint())
	{
	}

	//具材を皿の上に移動させる
	if (m_isHavingGuzai == true) {
		m_guzaiPos = m_position;
		m_guzaiPos.y += m_guzaiYPos;
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
				se->SetVolume(SE_VOLUME);
				se->Play(false);
				m_dishGene->SetFallingSound(true);
			}
		}
	}
	else {
		m_guzaiYPos = DROP_POS_TOP;
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
		}
	}
	else {
		if (m_dishGene->GetSetSound() == false) {
			//音を鳴らす
			CSoundSource* se = NewGO<CSoundSource>(0);
			se->Init(L"Assets/sound/thi-n.wav", false);
			se->SetVolume(SE_VOLUME);
			se->Play(false);
			m_dishGene->SetSetSound(true);
		}
	}
	
	//自分の上の具材が持たれているならば
	if (m_guzai.back()->GetisHadState() == true) {
		m_isHavingGuzai = false;
	}

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
				m_dishGene->SetSetSound(false);
			}
			m_guzaiTimer = 0;
		}
	}

	if (DishManager::GetInstance().GetSpeedUpState() == true/*m_speedManeger->GetSpeedUpState() == true*/) {
		float moveSpeed = MOVE_SPEED * 2.0f;
		m_pathMove.get()->ChangeMoveSpeed(moveSpeed);
	}
	else{
		m_pathMove.get()->ChangeMoveSpeed(MOVE_SPEED);
	}

	Move();

	m_skinModelRender->SetScale(m_scale);
}