#include "stdafx.h"
#include "Dish.h"
#include "SkinModelRender.h"
#include "Level.h"
#include "Guzai.h"
#include "PlayerGene.h"
#include "PathMove.h"
#include "DishSpeedManeger.h"
#include "SoundSource.h"
#include "DishGene.h"
#include "GuzaiGene.h"

namespace
{
	const float MOVE_SPEED = 130.0f;
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
	DeleteGO(m_skinModelRender);
	

	if (m_guzai != nullptr) {
		DeleteGO(m_guzai);
	}
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
	m_skinModelRender->InitShader("Assets/shader/model.fx", "VSMain", "VSSkinMain", DXGI_FORMAT_R32G32B32A32_FLOAT);

	
	m_playerGene = FindGO<PlayerGene>("playerGene");
	m_speedManeger = FindGO<DishSpeedManeger>("speedManeger");
	m_dishGene = FindGO<DishGene>("dishGene");
	m_guzaiGene = FindGO<GuzaiGene>("GuzaiGene");

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
		//m_guzaiPos.y += 10.0f;
		m_guzaiPos.y += m_guzaiYPos;
		m_guzaiGene->m_guzai[m_guzaiNo]->SetPosition(m_guzaiPos);
		//再ポップが行われて、皿のかなり高い位置にあるとき
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
			for (int i = 0; i < MAX_GUZAI_POP_NUM; i++) {
				//空いている箇所を見つける
				if (m_guzaiGene->GetGuzaiFlag(i) == false) {
					m_guzaiGene->m_guzai[i] = NewGO<Guzai>(0);
					m_guzaiGene->m_guzai[i]->SetGuzaiNo(i);
					m_isHavingGuzai = true;
					m_guzaiNo = i;
					m_isCompletedFirstPop = true;
					m_guzaiGene->SetGuzaiFlag(i, true);
					//1回だけ出すため
					break;
				}
			}
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
	if (m_guzaiGene->m_guzai[m_guzaiNo]->GetisHadState() == true) {
		m_isHavingGuzai = false;
	}

	//空の皿が規定数より多いので、空の皿だけに補充を開始する。
	if (m_playerGene->GetNoHavingDishCounter() >= m_maxNum2Refill) {
		m_guzaiTimer++;
		if (m_guzaiTimer > GUZAI_POP_DELAY) {
			if (m_isHavingGuzai == false) {
				for (int i = 0; i < MAX_GUZAI_POP_NUM; i++) {
					if (m_guzaiGene->GetGuzaiFlag(i) == false) {
						m_guzaiGene->m_guzai[i] = NewGO<Guzai>(0);
						m_guzaiGene->m_guzai[i]->SetGuzaiNo(i);
						m_isHavingGuzai = true;
						m_guzaiNo = i;
						m_guzaiGene->SetGuzaiFlag(i, true);
						break;
					}
				}

				//補充した皿の枚数を１足す
				m_playerGene->AddRefilledNum();
			}
			//補充した皿の数が空だった皿の数と同じになったら、０で初期化
			if (m_playerGene->GetRefilledNum() >= m_maxNum2Refill) {
				//空の皿は０のはずだから、カウントをリセットする。
				m_playerGene->ResetNohavingDishCounter();
				//次に具材を補充するときに、０からカウントしたいためカウントをリセットする。
				m_playerGene->ResetRefilledNum();
				m_dishGene->SetFallingSound(false);
				m_dishGene->SetSetSound(false);
			}
			m_guzaiTimer = 0;
		}
	}

	if (m_speedManeger->GetSpeedUpState() == true) {
		float moveSpeed = MOVE_SPEED * 2.0f;
		m_pathMove.get()->ChangeMoveSpeed(moveSpeed);
	}
	if (m_speedManeger->GetSpeedUpState() == false) {
		m_pathMove.get()->ChangeMoveSpeed(MOVE_SPEED);
	}


	Move();

	m_skinModelRender->SetScale(m_scale);
}