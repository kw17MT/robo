#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"
#include "MissileTargetIcon.h"
#include "CrossIcon.h"
#include "CapturedSquareIcon.h"

//したからFindGO対象　エネミーの数分スタートでFind
#include "Reticle.h"
#include "MissileGenerator.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

namespace
{
	const float FAR_DISTANCE = 80000.0f;					//ロックオン可能な距離。四角の画像を出すタイミングの距離
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//捕捉レティクルの収縮率
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//捕捉レティクルの初期拡大率
}

EnemyStateIcon::~EnemyStateIcon()
{
	DeleteGO(m_crossIcon); m_crossIcon = nullptr;
	DeleteGO(m_squareIcon); m_squareIcon = nullptr;

	/*for (int i = 0; i < 10; i++)
	{
		DeleteGO(m_missileTargetIcon[i]);
	}*/
}

bool EnemyStateIcon::Start()
{
	m_crossIcon = NewGO<CrossIcon>(0);
	m_squareIcon = NewGO<CapturedSquareIcon>(0);

	//m_reticle = FindGO<Reticle>("reticle");
	m_missileGenerator = FindGO<MissileGenerator>("missileGene");

	//for (int i = 0; i < 10; i++)
	//{
	//	m_missileTargetIcon[i] = NewGO<MissileTargetIcon>(0);
	//}

	for (auto i : m_missileTargetIcon2)
	{
		DeleteGO(m_missileTargetIcon2.back());
		m_missileTargetIcon2.pop_back();
	}
	m_missileTargetIcon2.clear();

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

void EnemyStateIcon::DisplayIcons()
{
	//スクリーン座標が画面の中央付近なら
	if (m_screenPos.x > -80.0f && m_screenPos.x < 80.0f && m_screenPos.y > -25.0f && m_screenPos.y < 25.0f)
	{
		//プレイヤーが何も捕捉していなければ
		if (CaptureStateManager::GetInstance().GetCaptureState() == None
			&& m_enemyState == enemyNear) {
			//捕捉レティクル（四角）をだす。
			m_isCaptured = true;
			m_isFirstExpand = false;
			//捕捉された敵がいることを保存
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}

		//ロケットのターゲットをする命令が来ていたら
		if (CaptureStateManager::GetInstance().GetRocketTargetState()) {
			//現在、この敵の位置座標はマネージャーが持つ配列の何番目に保存しているか把握しておく
			m_rocketTargetPosNumber.push_back(CaptureStateManager::GetInstance().GetRocketTargetNum());
			//ロケットにターゲットされた敵の数を配列の要素数に利用する
			m_missileTargetIcon2.push_back(NewGO<MissileTargetIcon>(0));
			m_missileTargetIcon2.back()->SetFirstExpandScale(true);
			m_missileTargetIcon2.back()->SetTargetedEnemy(m_enemy);

			//m_missileTargetIcon[m_rocketTargetPosNumber.back()]->SetFirstExpandScale(true);
			//m_missileTargetIcon[m_rocketTargetPosNumber.back()]->SetTargetedEnemy(m_enemy);
			CaptureStateManager::GetInstance().SetRocketTargetedEnemy(m_enemy);
			CaptureStateManager::GetInstance().PlusRockeTargetNum();
			CaptureStateManager::GetInstance().SetRocketTargetState(false);
		}
	}
	//範囲からはずれて
	else if(m_screenPos.x > -200.0f && m_screenPos.x < 200.0f && m_screenPos.y > -150.0f && m_screenPos.y < 150.0f)
	{
		//それが捕捉中の敵ならば
		if (m_isCaptured)
		{
			//捕捉状態解除
			m_isCaptured = false;
			//捕捉されている敵はいない状態にする。
			CaptureStateManager::GetInstance().SetCaptureState(None);

			//ロックオンしていた敵の座標を書き換える
			//Vector3 frontRobo =  g_camera3D->GetTarget() - g_camera3D->GetPosition();
			//frontRobo.Normalize();
			//CaptureStateManager::GetInstance().SetCapturedEnemyPos(frontRobo * 10.0f);
		}
		//プレイヤーは誰かしらをロックオンしているなら、次のロックオン先を予約しておく
		if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted
			&& CaptureStateManager::GetInstance().IsNextEnemyCaptured() == false) {
			//自分自身を再び次のロックオン対象にしないようにする。
			if (m_enemyState != enemyTargeted) {
				//次の予約はこの敵にする
				m_nextTarget = true;
				//予約完了
				CaptureStateManager::GetInstance().SetNextEnemy(true);
				//予約先の位置座標を保存
				CaptureStateManager::GetInstance().SetNextEnemyPos(m_enemyPos);
			}
		}

		//捕捉レティクルの初期の拡大をもう一度したいのでフラグを戻す
		m_isFirstExpand = false;
		//捕捉レティクル縮小
		m_scale[1] = Vector3::Zero;
	}
	//画面端付近のエリア
	else
	{
		if (m_nextTarget)
		{
			m_nextTarget = false;
			CaptureStateManager::GetInstance().SetNextEnemy(false);
			//予約先の位置座標を保存
			Vector3 homePos = { 0.0f,0.0f,-1.0f };
			CaptureStateManager::GetInstance().SetNextEnemyPos(homePos);
		}
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
				m_squareIcon->SetScale(m_scale[1]);
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
	//最初に敵のスクリーン座標を更新する。
	CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	
	DisplayIcons();

	//敵とプレイヤーの距離によって振る舞いを変える。
	IconBehaviour();

	//予約していた敵に照準を当てるように命令が来ていたら
	if (CaptureStateManager::GetInstance().GetCaptureState() == ChangeMainTarget)
	{
		//自身は予約されていたか
		if (m_nextTarget)
		{
			//予約を消し、
			m_nextTarget = false;
			//捕捉されている
			m_isCaptured = false;
			m_enemyState = enemyTargeted;
			//プレイヤーはターゲットしている
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
	}

	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		m_nextTarget = false;
	}

	//マネージャー側に保存しているであろう対応した位置座標を更新する。
	for (int i = 0; i < m_rocketTargetPosNumber.size(); i++)
	{
		//第二引数を要素数に指定して、そこを現在の敵の位置で更新する。
		CaptureStateManager::GetInstance().SetRocketTargetPos(m_enemyPos, m_rocketTargetPosNumber[i]);
	}

	if (m_missileGenerator->GetDeleteMissileIcon())
	{
		for (auto i : m_missileTargetIcon2)
		{
			DeleteGO(m_missileTargetIcon2.back());
			m_missileTargetIcon2.pop_back();
		}
		m_missileTargetIcon2.clear();
	}

	//バツのレティクルの拡大率更新
	m_crossIcon->SetScale(m_scale[0]);
	//捕捉レティクルの拡大率更新
	m_squareIcon->SetScale(m_scale[1]);
	//レティクルを敵のスクリーン座標に置く。
	m_crossIcon->SetPosition(m_screenPos);
	m_squareIcon->SetPosition(m_screenPos);
}