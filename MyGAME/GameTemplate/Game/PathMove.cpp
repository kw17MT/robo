#include "stdafx.h"
#include "PathMove.h"
#include "PlayerGene.h"
#include "SoundSource.h"
#include "CycleDirection.h"
#include "DishManager.h"

namespace
{
	const float DISTANCE = 50.0f * 50.0f;
	const int LEFT_CYCLE_SPRITE = 0;
	const int RIGHT_CYCLE_SPRITE = 1;
	const int FIXED_CYCLE_SPRITE = 2;

	const float SE_VOLUME = 2.0f;
}

//これをつかっているところ探す
void PathMove::Init(const Vector3& pos, const float move, EnLane enLane)
{
	m_position = pos;
	m_moveSpeed = move;

	m_cycleDirection = enNormal;

	m_path = PathFactory::GetInstance().GetPath(enLane);

	m_point = m_path->GetNearPoint(m_position);
}

void PathMove::SwitchCycleDirection()
{
	//ここで巻き戻るバグがあると思う
	//逆転方向に変える
	if (m_cycleDirection == enNormal) {
		m_cycleDirection = enReverse;
		//m_pointは次の目的地のウェイポイント番号であるから、巻き戻るには2マイナスする
		m_point = m_path->GetPoint(m_point->s_number - 2);
		//ウェイポイント番号は1~36の36個
		if (m_point->s_number <= 0) {
			m_point = m_path->GetPoint(m_wayPointNum-1);
		}
		Vector3 nextDistance = m_point->s_vector - m_position;
		nextDistance.Normalize();
		m_moveVector = nextDistance;

		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/mechanical.wav", false);
		se->SetVolume(2.0f);
		se->Play(false);
	}
	else {
		m_cycleDirection = enNormal;
		m_point = m_path->GetPoint(m_point->s_number);
		if (m_point->s_number <= 0) {
			m_point = m_path->GetPoint(1);
		}
		Vector3 nextDistance = m_point->s_vector - m_position;
		nextDistance.Normalize();
		m_moveVector = nextDistance;

		

		CSoundSource* se = NewGO<CSoundSource>(0);
		se->Init(L"Assets/sound/mechanical.wav", false);
		se->SetVolume(SE_VOLUME);
		se->Play(false);
	}
}

//ここをスプライトのみの変更関数を作成する。
void PathMove::SwitchCycleSpriteDirection()
{
	//逆転方向に変える
	if (m_cycleDirection == enNormal) {
		m_cycleDirection = enReverse;
		//回転方向スプライト(逆転)
		m_directSprite[LEFT_CYCLE_SPRITE] = FindGO<CycleDirection>("dirsp1");
		m_directSprite[LEFT_CYCLE_SPRITE]->ChangeSpriteForward();
		m_directSprite[LEFT_CYCLE_SPRITE]->SetDirection(Forward);					//右回転
		m_directSprite[LEFT_CYCLE_SPRITE]->SetSide(Left);							//左

		m_directSprite[RIGHT_CYCLE_SPRITE] = FindGO<CycleDirection>("dirsp2");
		m_directSprite[RIGHT_CYCLE_SPRITE]->ChangeSpriteReverse();
		m_directSprite[RIGHT_CYCLE_SPRITE]->SetDirection(Reverse);					//左回転
		m_directSprite[RIGHT_CYCLE_SPRITE]->SetSide(Right);							//右

		m_directSprite[FIXED_CYCLE_SPRITE] = FindGO<CycleDirection>("dirspfixed");	//固定表示
		m_directSprite[FIXED_CYCLE_SPRITE]->ChangeFixedSpriteForward();				//正転
		m_directSprite[FIXED_CYCLE_SPRITE]->SetDirection(FixedReverse);
	}
	else {
		m_cycleDirection = enNormal;
		//回転方向スプライト(正転)
		m_directSprite[LEFT_CYCLE_SPRITE] = FindGO<CycleDirection>("dirsp1");
		m_directSprite[LEFT_CYCLE_SPRITE]->ChangeSpriteReverse();
		m_directSprite[LEFT_CYCLE_SPRITE]->SetDirection(Reverse);					//左回転
		m_directSprite[LEFT_CYCLE_SPRITE]->SetSide(Left);							//左

		m_directSprite[RIGHT_CYCLE_SPRITE] = FindGO<CycleDirection>("dirsp2");
		m_directSprite[RIGHT_CYCLE_SPRITE]->ChangeSpriteForward();
		m_directSprite[RIGHT_CYCLE_SPRITE]->SetDirection(Forward);					//右回転
		m_directSprite[RIGHT_CYCLE_SPRITE]->SetSide(Right);							//右

		m_directSprite[FIXED_CYCLE_SPRITE] = FindGO<CycleDirection>("dirspfixed");	//固定表示
		m_directSprite[FIXED_CYCLE_SPRITE]->ChangeFixedSpriteReverse();
		m_directSprite[FIXED_CYCLE_SPRITE]->SetDirection(FixedForward);
	}
}

const Vector3& PathMove::Move()
{

		if (m_enMoveState == enStart) {
			m_moveVector = m_point->s_vector - m_position;
			m_moveVector.Normalize();
			m_enMoveState = enMove;
			return m_position;
		}

		//皿の流れる向きを変えろと言われたら
		if (DishManager::GetInstance().GetIsOrderedDirection()) {
			//向きを変える。
			SwitchCycleDirection();
			//向きを変えた回数を記録する（＋１する）
			DishManager::GetInstance().AddCompletedChangeDirectionNum();

			//向きを変えた回数が皿の総数と一致したならば
			if (DishManager::GetInstance().JudgeChangedAllDirection()) {
				//画像の向きだけを変える。
				SwitchCycleSpriteDirection();
				//全部一致したので、向きを変える命令を止める
				DishManager::GetInstance().SetOrderChangeDirection(false);
				DishManager::GetInstance().ResetCompletedChangeDirectionNum();
			}
		}
		
		Vector3 distance = m_point->s_vector - m_position;
		if (distance.LengthSq() <= DISTANCE) {
			//次のパスに向けての移動ベクトルを求める
			if (m_cycleDirection == enNormal) {
				m_point = m_path->GetPoint(m_point->s_number);
				if (m_point->s_number <= 0) {
					m_point = m_path->GetPoint(1);
				}
				Vector3 nextDistance = m_point->s_vector - m_position;
				nextDistance.Normalize();
				m_moveVector = nextDistance;
			}
			//ここで具材の流れを反転させる
			else if (m_cycleDirection == enReverse) {
				m_point = m_path->GetPoint(m_point->s_number - 2);
				if (m_point->s_number <= 0) {
					m_point = m_path->GetPoint(m_wayPointNum - 1);
				}
				Vector3 nextDistance = m_point->s_vector - m_position;
				nextDistance.Normalize();
				m_moveVector = nextDistance;
			}
		}

	//TODO GameTimeに変える。
		float gameTime = GameTime().GetFrameDeltaTime();

	m_position += m_moveVector * gameTime * m_moveSpeed;
	return m_position;
}
