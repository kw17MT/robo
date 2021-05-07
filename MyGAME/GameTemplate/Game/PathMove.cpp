#include "stdafx.h"
#include "PathMove.h"

namespace
{
	const float DISTANCE = 50.0f * 50.0f;

}

void PathMove::Init(const Vector3& pos, const float move, EnLane enLane)
{
	
	m_position = pos;
	m_moveSpeed = move;

	
	m_position = pos;
	m_moveSpeed = move;
	m_path = PathFactory::GetInstance().GetPath(enLane);
	//一番最初のポイントを取得する
	//m_point = m_path->GetFirstPoint();
	m_point = m_path->GetNearPoint(m_position);

}

const Vector3& PathMove::Move()
{
	////最終ポイントに到達した、何もステートが設定されてなかったら。
	//if (m_enMoveState == enFinal || m_enMoveState == enNone)
	//{
	//	//処理しない。
	//	return m_position;
	//}

	if (m_enMoveState == enStart) {
		m_moveVector = m_point->s_vector - m_position;
		m_moveVector.Normalize();
		m_enMoveState = enMove;
		return m_position;
	}
	
	Vector3 distance = m_point->s_vector - m_position;
	if (distance.LengthSq() <= DISTANCE) {
		/*if (m_path->GetIsFinalPoint(m_point->s_number))
		{
			m_enMoveState = enFinal;
			return m_position;
		}*/
		//次のパスに向けての移動ベクトルを求める
		m_point = m_path->GetPoint(m_point->s_number);
		Vector3 nextDistance = m_point->s_vector - m_position;
		nextDistance.Normalize();
		m_moveVector = nextDistance;
	}

	//TODO GameTimeに変える。
	m_position += m_moveVector * 1.0f / 60.0f * m_moveSpeed;
	return m_position;
}
