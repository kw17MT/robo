#include "stdafx.h"
#include "EnemyGenerator.h"
#include "Enemy.h"
#include "GameDirector.h"
#include "EnemyRepopManager.h"

namespace
{
	const int MAX_ENEMY_NUM = 10;
	const float ENEMY_SPACE = 1000.0f;
}

EnemyGenerator::~EnemyGenerator()
{
	//���������G�̍폜
	for (auto i  : m_enemys)
	{
		DeleteGO(m_enemys.back());
		m_enemys.pop_back();
	}
	m_enemys.clear();
}

bool EnemyGenerator::Start()
{
	//�G���ő吔�쐬
	for (int i = 0; i < MAX_ENEMY_NUM; i++)
	{
		m_enemys.push_back(NewGO<Enemy>(0, "enemy"));

		Vector3 enemyPos = m_spawnPoint.GetNorthPosition();
		Vector3 toEnemyPos = enemyPos - Vector3::Zero;
		toEnemyPos.Normalize();
		toEnemyPos.Cross({ 0.0f,1.0f,0.0f });

		//�������ꂽ�ԍ��ňʒu�����炷
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
	//�Q�[������������
	if (GameDirector::GetInstance().GetGameScene() == enInGame)
	{
		//�x�[�X�ƂȂ�G�̃|�b�v�ʒu�����肷��
		Vector3 baseEnemyPos = m_spawnPoint.DecideSpawnPoint();
		//���_����x�[�X�̃|�b�v�ʒu�ւ̕����x�N�g�����v�Z����
		Vector3 toEnemyPos = baseEnemyPos - Vector3::Zero;
		//���K������
		toEnemyPos.Normalize();
		//���K���x�N�g���ƃ��[���h�̏�����Ƃ̊O�ς����߂ĉ��x�N�g�����v�Z����
		toEnemyPos.Cross({ 0.0f,1.0f,0.0f });
		//�G���ő吔�쐬
		for (int i = 0; i < MAX_ENEMY_NUM; i++)
		{
			//�ő吔�܂œG���|�b�v
			m_enemys.push_back(NewGO<Enemy>(0, "enemy"));
			//���ۂɓG�ɃZ�b�g������W
			Vector3 enemyPos = baseEnemyPos;
			//��ʒu�����ƂɈʒu�����炷
			enemyPos += (toEnemyPos * ENEMY_SPACE) * (i - 5);
			//�G�Ƀ|�b�v�ʒu��`����
			m_enemys.back()->SetPosition(enemyPos);
		}
		//�|�b�v�����G�̐���ۑ�����
		EnemyRepopManager::GetInstance().SetPopedEnemyNum(MAX_ENEMY_NUM);
	}
}

void EnemyGenerator::Update()
{
	//int AliveEnemyNum = m_enemys.size() - 1;

	//for (auto enemy : m_enemys)
	//{
	//	if (enemy != nullptr && enemy->IsDead())
	//	{
	//		AliveEnemyNum--;
	//		if (AliveEnemyNum == 0)
	//		{
	//			CleanUpArray();
	//			GenerateEnemy();
	//		}
	//	}
	//}

	if (EnemyRepopManager::GetInstance().ShouldRepopEnemy())
	{
		CleanUpArray();
		GenerateEnemy();
	}
}