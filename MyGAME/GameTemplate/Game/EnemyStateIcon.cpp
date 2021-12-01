#include "stdafx.h"
#include "EnemyStateIcon.h"
#include "SpriteRender.h"
#include "CaptureStateManager.h"
#include "MissileTargetIcon.h"
#include "CrossIcon.h"
#include "CapturedSquareIcon.h"
#include "Reticle.h"

//FindGO対象
#include "MissileGenerator.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

namespace
{
	const float FAR_DISTANCE = 80000.0f;					//ロックオン可能な距離。四角の画像を出すタイミングの距離
	const Vector3 SHRINK_RATE = { 0.1f,0.1f,0.1f };			//捕捉レティクルの収縮率
	const Vector3 APPEAR_RATE = { 2.0f,2.0f,2.0f };			//捕捉レティクルの初期拡大率
	const float CAPTURE_HEIGHT = 80.0f;						//捕捉できる画面上の高さ
	const float CAPTURE_WIDTH = 45.0f;						//捕捉できる画面上の横幅
	const float NEXT_TARGET_AREA_HEIGHT = 200.0f;			//次自動ターゲットする判定を行うエリアの高さ
	const float NEXT_TARGET_AREA_WIDTH = 150.0f;			//次自動ターゲットする判定を行うエリアの横幅
}

EnemyStateIcon::~EnemyStateIcon()
{
	//バツ画像を削除
	DeleteGO(m_crossIcon); m_crossIcon = nullptr;
	//捕捉の画像を削除
	DeleteGO(m_squareIcon); m_squareIcon = nullptr;

	//ミサイルのターゲットアイコンを全削除
	for (auto i : m_missileTargetIcon)
	{
		DeleteGO(m_missileTargetIcon.back());
		m_missileTargetIcon.pop_back();
	}
	m_missileTargetIcon.clear();

	if (m_nextTarget)
	{
		//m_reticle->SetIsDecidedNextTarget(false);
		CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
	}
	if (m_isCaptured)
	{
		//m_reticle->SetIsDecidedNextTarget(false);
		CaptureStateManager::GetInstance().SetCaptureState(None);
	}
	if (m_enemyState == enemyTargeted)
	{
		CaptureStateManager::GetInstance().SetCaptureState(ChangeMainTarget);
	}
}

bool EnemyStateIcon::Start()
{
	//バツ画像を生成
	m_crossIcon = NewGO<CrossIcon>(0);
	//捕捉画像を生成
	m_squareIcon = NewGO<CapturedSquareIcon>(0);
	//ミサイル生成器を作成
	m_missileGenerator = FindGO<MissileGenerator>("missileGene");

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
	if (m_screenPos.x > -CAPTURE_HEIGHT && m_screenPos.x < CAPTURE_HEIGHT 
		&& m_screenPos.y > -CAPTURE_WIDTH && m_screenPos.y < CAPTURE_WIDTH)
	{
		//プレイヤーが何も捕捉していなければ
		if (CaptureStateManager::GetInstance().GetCaptureState() == None
			&& m_enemyState == enemyNear) {
			//捕捉レティクル（四角）をだす。
			m_isCaptured = true;
			m_isFinishedFirstExpand = false;
			//捕捉された敵がいることを保存
			CaptureStateManager::GetInstance().SetCaptureState(Captured);
		}

		//ロケットのターゲットをする命令が来ていたら
		if (CaptureStateManager::GetInstance().GetMissileTargetState() == enMissileTarget) {
			//ミサイルがもっと出せる状態ならば
			if (m_missileGenerator->CanTargetMore())
			{
				//ターゲットする敵のインスタンスを保存
				m_missileGenerator->SaveTargetedEnemy(m_enemy);
				
				//ロケットにターゲットされた敵の数を配列の要素数に利用する
				m_missileTargetIcon.push_back(NewGO<MissileTargetIcon>(0));
				//最初の拡大を行う
				m_missileTargetIcon.back()->SetFirstExpandScale(true);
				//アイコンを出す対象の敵のインスタンスを保存
				m_missileTargetIcon.back()->SetTargetedEnemy(m_enemy);
				//ミサイルのターゲットを行った
				CaptureStateManager::GetInstance().SetMissileTargetState(enMissileTargeted);
			}
			//ミサイルはもう出せない場合
			else
			{
				//最大限ミサイルロックオンした状態にする。
				CaptureStateManager::GetInstance().SetMissileTargetState(enFull);
			}
		}
	}
	//範囲からはずれて
	else if(m_screenPos.x > -NEXT_TARGET_AREA_HEIGHT && m_screenPos.x < NEXT_TARGET_AREA_HEIGHT 
		&& m_screenPos.y > -NEXT_TARGET_AREA_WIDTH && m_screenPos.y < NEXT_TARGET_AREA_WIDTH)
	{
		//それが捕捉中の敵ならば
		if (m_isCaptured)
		{
			//捕捉状態解除
			m_isCaptured = false;
			//捕捉されている敵はいない状態にする。
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		//プレイヤーは誰かしらをロックオンしているなら、次のロックオン先を予約しておく
		if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted
			&& !CaptureStateManager::GetInstance().GetIsDecidedNextTarget()/*m_reticle->GetIsDecidedNextTarget() == false*/) {
			//自分自身を再び次のロックオン対象にしないようにする。
			if (m_enemyState != enemyTargeted) {
				//次の予約はこの敵にする
				m_nextTarget = true;
				//m_reticle->SetIsDecidedNextTarget(true);
				CaptureStateManager::GetInstance().SetIsDecidedNextTarget(true);
			}
		}

		//捕捉レティクルの初期の拡大をもう一度したいのでフラグを戻す
		m_isFinishedFirstExpand = false;
		//捕捉レティクル縮小
		m_scale[1] = Vector3::Zero;
	}
	//画面端付近のエリア
	else
	{
		if (m_nextTarget)
		{
			m_nextTarget = false;
			
			//m_reticle->SetIsDecidedNextTarget(false);
			CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
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
			if (m_isFinishedFirstExpand == false)
			{
				//大きめの拡大率に更新
				m_scale[1] = APPEAR_RATE;
				//距離が近くなったら最初だけ少し大きめにする。
				m_squareIcon->SetScale(m_scale[1]);
				//最初の拡大が終わった
				m_isFinishedFirstExpand = true;
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
		m_isFinishedFirstExpand = false;
		if (m_isCaptured) {
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}
		m_isCaptured = false;
		break;

	case enemyTargeted:
		//このアイコンが追随している敵がロックオンされたので2種類のアイコンをけす。
		m_scale[0] = Vector3::Zero;
		m_scale[1] = Vector3::Zero;
		m_isFinishedFirstExpand = false;
		m_isCaptured = false;

		m_reticle->SetIsTargeting(true);

		break;
	}
}

void EnemyStateIcon::Update()
{
	//最初に敵のスクリーン座標を更新する。
	CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	
	//アイコンを表示する
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
			//ターゲットされている
			m_enemyState = enemyTargeted;
			//レティクルはターゲット中
			m_reticle->SetIsTargeting(true);
			//m_reticle->SetIsDecidedNextTarget(false);
			CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
			//プレイヤーはターゲットしている
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
	}
	//ミサイルのアイコンを消去するべきならば
	if (m_missileGenerator->GetDeleteMissileIcon())
	{
		//アイコンを全消去
		for (auto i : m_missileTargetIcon)
		{
			DeleteGO(m_missileTargetIcon.back());
			m_missileTargetIcon.pop_back();
		}
		m_missileTargetIcon.clear();
	}
	//プレイヤーは何もターゲットや捕捉していなければ
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		//次にターゲットする対象にはしない
		m_nextTarget = false;
		CaptureStateManager::GetInstance().SetIsDecidedNextTarget(false);
	}

	if (m_enemyState == enemyTargeted)
	{
		//この敵がターゲットされているならば、現在位置を送る
		m_reticle->SetTargetedEnemyPos(m_enemy->GetPosition());
	}

	//バツのレティクルの拡大率更新
	m_crossIcon->SetScale(m_scale[0]);
	//捕捉レティクルの拡大率更新
	m_squareIcon->SetScale(m_scale[1]);
	//レティクルを敵のスクリーン座標に置く。
	m_crossIcon->SetPosition(m_screenPos);
	m_squareIcon->SetPosition(m_screenPos);
}