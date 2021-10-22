#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"

namespace
{
	const float FAR_DISTANCE = 80000.0f;					//ロックオン可能な距離。四角の画像を出すタイミングの距離
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//捕捉レティクルの収縮率
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//捕捉レティクルの初期拡大率
}

EnemyStateIcon::~EnemyStateIcon()
{
	DeleteGO(m_spriteRender[0]); m_spriteRender[0] = nullptr;
	DeleteGO(m_spriteRender[1]); m_spriteRender[1] = nullptr;
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
	//他の敵がロックオンされている時は他の敵の状態を変えない
	if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
	{
		return;
	}
	//ロックオンできる距離ならば
	if (distance < FAR_DISTANCE)                              
	{
		m_enemyState = enemyNear;
	}
	//ロックオンできない距離ならば
	else if(distance >= FAR_DISTANCE)
	{
		m_enemyState = enemyTooFar;
	}
}

void EnemyStateIcon::CalcPosition()
{
	//対応する敵のポジションからスクリーン座標を求める
	g_camera3D->CalcScreenPositionFromWorldPosition(m_screenPos, m_enemyPos);
	m_position.x = -m_screenPos.x;
	m_position.y = m_screenPos.y;
	//敵の位置とカメラの前方向の内積によってアイコンを映すか決める
	//敵からカメラへのベクトル作成
	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//正規化
	enemyToCamera.Normalize();
	//敵の位置とカメラの前方向の内積
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//敵がカメラの前方向にあるならば映す
	if (dot < 0.0f)
	{
		m_position.z = 0.0f;
	}
	//後ろ側にある
	else
	{
		m_position.z = -1.0f;
	}
}

void EnemyStateIcon::DisplayIcons()
{
	//スクリーン座標が画面の中央付近なら
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f)
	{
		//プレイヤーが何も捕捉していなければ
		if (CaptureStateManager::GetInstance().GetCaptureState() == None) {
			//捕捉レティクル（四角）をだす。
			m_isCaptured = true;
			m_isFirstExpand = false;
			//捕捉された敵がいることと、その位置座標を保存
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}
	}
	//範囲からはずれて
	else
	{
		//それが捕捉中の敵ならば
		if (m_isCaptured)
		{
			//捕捉状態解除
			m_isCaptured = false;
			//捕捉されている敵はいない状態にする。
			CaptureStateManager::GetInstance().SetCaptureState(None);

			Vector3 frontRobo =  g_camera3D->GetTarget() - g_camera3D->GetPosition();
			frontRobo.Normalize();
			CaptureStateManager::GetInstance().SetCapturedEnemyPos(frontRobo * 10.0f);
		}

		//捕捉レティクルの初期の拡大をもう一度したいのでフラグを戻す
		m_isFirstExpand = false;
		//捕捉レティクル縮小
		m_scale[1] = Vector3::Zero;
	}
}

void EnemyStateIcon::IconBehaviour()
{
	//バツアイコンは基本出す。ロックオンされた時に拡大率を更新する。
	m_scale[0] = Vector3::One;

	switch (m_enemyState)
	{
		//自分はロックオンされる十分な距離の中で、捕捉されている時
	case enemyNear:
		if (m_isCaptured)
		{
			//始めの拡大率を大きくしておく
			if (m_isFirstExpand == false)
			{
				//大きめの拡大率に更新
				m_scale[1] = APPEAR_RATE;
				//距離が近くなったら最初だけ少し大きめにする。
				m_spriteRender[1]->SetScale(m_scale[1]);
				//最初の拡大が終わった
				m_isFirstExpand = true;
				return;
			}
			//大きくしていたものをだんだん小さくしていく。
			m_scale[1] -= SHRINK_RATE;
			if (m_scale[1].x < 1.0f)
			{
				m_scale[1] = Vector3::One;
			}

			//プレイヤーが敵をターゲットしているならばそれは自分
			if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
			{
				m_enemyState = enemyTargeted;
			}
		}
		break;

	case enemyTooFar:
		//敵が遠くなったので捕捉レティクルの拡大率をゼロにして消す
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		if (m_isCaptured) {
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		m_isCaptured = false;
		break;

	case enemyTargeted:
		//このアイコンが追随している敵がロックオンされたので2種類のアイコンをけす。
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFirstExpand = false;
		m_isCaptured = false;
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(m_enemyPos);
		break;
	}
}

void EnemyStateIcon::Update()
{
	CalcPosition();
	
	DisplayIcons();

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