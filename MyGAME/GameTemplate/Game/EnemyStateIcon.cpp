#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"

namespace
{
	const int FAR_DISTANCE = 80000;							//ロックオン可能な距離。四角の画像を出すタイミングの距離
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//捕捉レティクルの収縮率
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//捕捉レティクルの初期拡大率
}

bool EnemyStateIcon::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_toofar.dds",48,48);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_near2.dds",48,48);

	return true;
}

void EnemyStateIcon::JudgeState(int distance)
{
	if (distance < FAR_DISTANCE)                              
	{
		m_enemyState = enemyNear;
	}
	else if(distance >= FAR_DISTANCE)
	{
		m_enemyState = enemyTooFar;
	}
}

void EnemyStateIcon::IconBehaviour()
{
	switch (m_enemyState)
	{
	case enemyNear:
		if (m_isCaptured && CaptureStateManager::GetInstance().GetOtherCapturedState())
		{
			//始めの拡大率を大きくしておく
			if (m_isFirstExpand == false)
			{
				//距離が近くなったら最初だけ少し大きめにする。
				m_spriteRender[1]->SetScale(APPEAR_RATE);
				//大きめの拡大率に更新
				m_scale[1] = APPEAR_RATE;
				//最初の拡大が終わった
				m_isFirstExpand = true;
				return;
			}
			//大きくしていたものをだんだん小さくしていく。
			m_scale[1] -= SHRINK_RATE;
			if (m_scale[1].x < 1)
			{
				m_scale[1] = Vector3::One;
			}
		}
		break;
	case enemyTooFar:
		//敵が遠くなったので捕捉レティクルの拡大率をゼロにして消す
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetOtherCapturedState(false, Vector3::Zero);
		break;
	case enemyTargeted:
		//このアイコンが追随している敵がロックオンされたので2種類のアイコンをけす。
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetOtherCapturedState(true, m_enemyPos);
		break;
	}
}

void EnemyStateIcon::Update()
{
	//対応する敵のポジションからスクリーン座標を求める
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, m_enemyPos);
	m_position.x = -m_screenPos.x;
	m_position.y = m_screenPos.y;
	//スクリーン座標が画面の中央付近なら
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f )
	{
		if (!CaptureStateManager::GetInstance().GetOtherCapturedState()) {
			//プレイヤーが何も捕捉していなければ
			//捕捉レティクル（四角）をだす。
			m_isCaptured = true;
			CaptureStateManager::GetInstance().SetOtherCapturedState(true, m_enemyPos);
		}
	}
	else 
	{
		//初期の拡大をもう一度したいのでフラグを戻す
		m_isFirstExpand = false;
		//縮小して消す
		m_scale[1] = Vector3::Zero;
		if (m_isCaptured)
		{
			CaptureStateManager::GetInstance().SetOtherCapturedState(false, Vector3::Zero);
		}
		//捕捉レティクルをださない。
		m_isCaptured = false;
	}

	//敵とプレイヤーの距離によって振る舞いを変える。
	IconBehaviour();

	//バツのレティクルの拡大率更新
	m_spriteRender[0]->SetScale(m_scale[0]);
	//捕捉レティクルの拡大率更新
	m_spriteRender[1]->SetScale(m_scale[1]);
	//レティクルを敵のスクリーン座標に置く。
	m_spriteRender[0]->SetPosition(m_position);
	m_spriteRender[1]->SetPosition(m_position);
}