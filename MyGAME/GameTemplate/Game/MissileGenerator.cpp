#include "stdafx.h"
#include "MissileGenerator.h"
#include "Missile.h"
#include "CaptureStateManager.h"

namespace
{
	const int WAIT_SECOND = 1.0f;
	const int MAX_LAUNCH_NUM = 10;
}

MissileGenerator::~MissileGenerator()
{
	for (auto i : m_missiles)
	{
		DeleteGO(m_missiles.back());
		m_missiles.pop_back();
	}
	m_missiles.clear();
}

bool MissileGenerator::Start()
{
	return true;
}

void MissileGenerator::Update()
{
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		////ロックオンの間隔を計測する
		//m_lockOnInterval += GameTime().GetFrameDeltaTime();

		////計測時間が一定以上になったら
		//if (m_lockOnInterval >= WAIT_SECOND)
		//{
		//	//発射するロケットの数を増やす
		//	m_launchNum++;

		//	//ここでロックオンした位置を設定していきたい

		//	//発射間隔時間をリセット
		//	m_lockOnInterval = 0.0f;
		//	//発射するミサイルの最大数になれば
		//	if (m_launchNum >= MAX_LAUNCH_NUM)
		//	{
		//		//最大数を維持
		//		m_launchNum = MAX_LAUNCH_NUM;
		//	}
		//}



		//発射の準備しているよ
		m_isPrepareLaunch = true;
	}
	//発射の準備ができていて、ボタンを離したら
	if (m_isPrepareLaunch && !g_pad[0]->IsPress(enButtonLB2))
	{
		//発射開始
		m_isLaunch = true;
	}
	//発射
	if (m_isLaunch)
	{
		//発射数分ロケットをだす
		m_launchNum = CaptureStateManager::GetInstance().GetRocketTargetNum();
		for (int i = 0; i < m_launchNum; i++)
		{
			m_missiles.push_back(NewGO<Missile>(0));
			//1発1発出現させる場所を変える
			Vector3 individualPos = m_launchPos;
			individualPos.x += i * 100.0f;
			m_missiles.back()->SetTargetAndCurrentPos(CaptureStateManager::GetInstance().GetRocketTargetEnemyPos(i), individualPos);
			m_missiles.back()->SetEnemy(CaptureStateManager::GetInstance().GetRocketTargetEnemy(i));
			m_missiles.back()->SetNumber(i);
		}

		//ロケットを発射したため、ロックオンしていた敵の数と場所をリセット
		CaptureStateManager::GetInstance().ResetRocketTargetParam();

		m_isPrepareLaunch = false;
		m_isLaunch = false;
	}
}