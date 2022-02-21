#include "stdafx.h"
#include "EnemyGenerator.h"
#include "Enemy.h"
#include "GameDirector.h"
#include "EnemyRepopManager.h"

namespace
{
	const int MAX_ENEMY_NUM = 10;				//一度に生成する敵の数
	const float ENEMY_SPACE = 1000.0f;			//生成する際の敵の間隔
}

EnemyGenerator::~EnemyGenerator()
{
	//生成した敵の削除
	for (auto i  : m_enemys)
	{
		DeleteGO(m_enemys.back());
		m_enemys.pop_back();
	}
	//敵配列を全消去
	m_enemys.clear();
}

bool EnemyGenerator::Start()
{
	//敵を最大数作成
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		m_enemys.push_back(NewGO<Enemy>(0, "enemy"));

		//最初に生成するときだけ北に生成
		Vector3 enemyPos = m_spawnPoint.GetNorthPosition();
		//ゲームの中心座標であるゼロから敵への方向
		Vector3 toEnemyPos = enemyPos - Vector3::Zero;
		toEnemyPos.Normalize();
		//最初に生成された敵の横方向を外積を用いて求める
		toEnemyPos.Cross({ 0.0f,1.0f,0.0f });

		//生成された番号で横方向に位置をずらす
		enemyPos += (toEnemyPos * ENEMY_SPACE) * (i - 5);
		m_enemys.back()->SetPosition(enemyPos);
	}
	EnemyRepopManager::GetInstance().SetPopedEnemyNum(MAX_ENEMY_NUM);

	return true;
}

Vector3 EnemyGenerator::GetEnemyPos(int no)
{
	return  m_enemys[no + 1]->GetPosition(); 
}

bool EnemyGenerator::GetIsEnemyAlive(int no)
{
	if (m_enemys[no + 1] == nullptr
		|| (m_enemys[no + 1] != nullptr && m_enemys[no + 1]->IsDead()))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void EnemyGenerator::CleanUpArray()
{
	for (auto enemy : m_enemys)
	{
		DeleteGO(m_enemys.back());
		m_enemys.pop_back();
	}
	m_enemys.clear();
	m_enemys.push_back(nullptr);
}

void EnemyGenerator::GenerateEnemy()
{
	//ゲーム中だったら
	if (GameDirector::GetInstance().GetGameScene() == enInGame)
	{
		//ベースとなる敵のポップ位置を決定する
		Vector3 baseEnemyPos = m_spawnPoint.DecideSpawnPoint();
		//原点からベースのポップ位置への方向ベクトルを計算する
		Vector3 toEnemyPos = baseEnemyPos - Vector3::Zero;
		//正規化する
		toEnemyPos.Normalize();
		//正規化ベクトルとワールドの上方向との外積を求めて横ベクトルを計算する
		toEnemyPos.Cross({ 0.0f,1.0f,0.0f });
		//敵を最大数作成
		for (int i = 0; i < MAX_ENEMY_NUM; i++)
		{
			//最大数まで敵をポップ
			m_enemys.push_back(NewGO<Enemy>(0, "enemy"));
			//実際に敵にセットする座標
			Vector3 enemyPos = baseEnemyPos;
			//基準位置をもとに位置をずらす
			enemyPos += (toEnemyPos * ENEMY_SPACE) * (i - 5);
			//敵にポップ位置を伝える
			m_enemys.back()->SetPosition(enemyPos);
		}
		//ポップした敵の数を保存する
		EnemyRepopManager::GetInstance().SetPopedEnemyNum(MAX_ENEMY_NUM);
	}
}

void EnemyGenerator::Update()
{
	//敵をリポップするならば、敵の配列をクリアして再生成
	if (EnemyRepopManager::GetInstance().ShouldRepopEnemy())
	{
		CleanUpArray();
		GenerateEnemy();
	}
}