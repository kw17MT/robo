#include "stdafx.h"
#include "Enemy.h"
#include "SkinModelRender.h"
#include "DisplayDistance.h"
#include "Player.h"
#include "EnemyStateIcon.h"
#include "CaptureStateManager.h"
#include "EnemyBrain.h"
#include "EnemyMachinGun.h"
#include "SoundSource.h"

namespace
{
	const float SE_VOLUME = 1.0f;
}

Enemy::~Enemy()
{
	CSoundSource* explodeSE = NewGO<CSoundSource>(0);
	explodeSE->Init(L"Assets/sound/explode.wav", false);
	explodeSE->SetVolume(SE_VOLUME);
	explodeSE->Play(false);

	Effect effect;
	effect.Init(u"Assets/effect/explosion2.efk");
	effect.SetScale({ 100.0f,100.0f,100.0f });
	effect.SetPosition(m_position);
	effect.Play();
	effect.Update();

	DeleteGO(m_skinModelRender);
	DeleteGO(m_displayDistance);
	DeleteGO(m_enemyStateIcon);
	DeleteGO(m_enemyHP);
	DeleteGO(m_machinGun);
}

void Enemy::TakenDamage(EnDamageTypes damageType)
{
	//HPにダメージを反映する。
	m_enemyHP->ApplyDamage(damageType);
}

bool Enemy::Start()
{
	//プレイヤーインスタンスの情報取得
	m_player = FindGO<Player>("player");

	//敵のモデルをインスタンス化
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/enemy/drone.tkm", nullptr, enModelUpAxisZ, true);
	m_skinModelRender->SetScale({ 15.0f, 15.0f, 15.0f});

	//距離（文字）を表示
	m_displayDistance = NewGO<DisplayDistance>(0);
	//位置を敵に設定
	m_displayDistance->SetEnemyPos(m_position);

	//敵にかかるアイコンを表示
	m_enemyStateIcon = NewGO<EnemyStateIcon>(0);
	//位置を敵に設定
	m_enemyStateIcon->SetEnemyPos(m_position);
	//プレイヤーと敵の距離を渡す
	m_enemyStateIcon->JudgeState(m_distance);

	m_enemyStateIcon->SetReticleInstance(m_player->GetReticleInstance());

	//HPバーを表示
	m_enemyHP = NewGO<EnemyHP>(0);
	//位置を敵に設定
	m_enemyHP->SetEnemyPos(m_position);

	//20～30のランダムな数値を作成。
	const float moveSpeed = rand() % 20 + 10.0f;
	m_enemyBrain.SetMoveSpeed(moveSpeed);

	m_machinGun = NewGO<EnemyMachinGun>(0);

	//モデルの位置を設定
	m_skinModelRender->SetPosition(m_position);
	return true;
}

void Enemy::Update()
{
	// 座標関係 //////////////////////////////////////////////////////////////////////////////////////
	m_enemyBrain.MemoryPlayerPos(m_player->GetPosition());
	m_enemyBrain.MemoryEnemyPos(m_position);
	m_enemyBrain.Work();
	//計算した次の位置座標を取得
	m_position = m_enemyBrain.GetNextEnemyPos();
	//位置を更新
	m_skinModelRender->SetPosition(m_position);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	// UI関係 ////////////////////////////////////////////////////////////////////////////////////////
	//プレイヤーと自分（敵）の距離を計測し、自分にもその情報を保存
	m_distance = m_displayDistance->CalcDistance(m_position, m_player->GetPosition());
	//自分についてくるレティクルに位置座標を与える
	m_enemyStateIcon->SetEnemyPos(m_position);
	//プレイヤーと自分の距離を与えてレティクルの状態を更新する。
	m_enemyStateIcon->JudgeState(m_distance);
	//自分がターゲットされていたらHPバーを出現させる。
	m_enemyHP->IsEnemyTargeted(m_enemyStateIcon->IsTargeted());
	//HPバーの位置を更新
	m_enemyHP->SetEnemyPos(m_position);
	//自分のインスタンスをアイコンに渡してやる
	m_enemyStateIcon->SetEnemyObject(this);
	//////////////////////////////////////////////////////////////////////////////////////////////////

	m_machinGun->SetPosition(m_position);
	if (m_enemyBrain.JudgeCanShoot())
	{
		m_machinGun->SetTargetPos(m_player->GetPosition());
		m_machinGun->SetCanShoot(true);
	}

	Effect effect;
	effect.Init(u"Assets/effect/enemyTrack3.efk");
	effect.SetPosition(m_position);
	effect.SetScale({ 20.0f,20.0f,20.0f });
	effect.Play();
	effect.Update();

	//HPがなくなったら
	if (m_enemyHP->IsEnemyDead())
	{
		//消す
		DeleteGO(this);
	}

	m_skinModelRender->SetRotation(m_enemyBrain.GetEnemyRotation());
}