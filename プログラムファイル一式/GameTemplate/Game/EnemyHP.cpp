#include "stdafx.h"
#include "EnemyHP.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_DAMAGE_AMOUNT = 10;				//マシンガンの弾のダメージ量
	const int MISSILE_DAMAGE_AMOUNT = 100;				//ミサイルのダメージ量
	const int RASER_DAMAGE_AMOUNT = 33;					//レールガンのダメージ量

	const float MAX_ENEMY_HP = 100.0f;					//敵の最大HP
	const int ENEMY_HPBAR_X = 128;						//HPバーの横幅
	const int ENEMY_HPBAR_Y = 32;						//HPバーの縦幅
	const Vector2 ENEMY_HPBAR_PIVOT = { 1.0f, 1.5f };	//HPバーの基点（画像の関係上すこし横幅基点を大きくとった）
}

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

EnemyHP::~EnemyHP()
{
	//HPバーのインスタンス破棄
	DeleteGO(m_spriteRender);
}

bool EnemyHP::Start()
{
	//HPバーのモデル生成
	m_spriteRender = NewGO<SpriteRender>(1);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds", ENEMY_HPBAR_X, ENEMY_HPBAR_Y);
	m_spriteRender->SetPivot(ENEMY_HPBAR_PIVOT);

	return true;
}

void EnemyHP::ApplyDamage(EnDamageTypes damageType)
{
	//ダメージのタイプによって残りHPを計算する
	switch (damageType)
	{
	case enBullet:
		m_enemyHP -= BULLET_DAMAGE_AMOUNT;
		break;
	case enMissile:
		m_enemyHP -= MISSILE_DAMAGE_AMOUNT;
		break;
	case enRaser:
		m_enemyHP -= RASER_DAMAGE_AMOUNT;
		break;
	}

	//HPが0以下になれば
	if (m_enemyHP <= 0.0f)
	{
		//倒されたことを記録
		m_isEnemyDead = true;
	}
}

void EnemyHP::Update()
{
	//ワールド座標から、カメラの行列をつかってスクリーン座標に変換する
	CalcMethods::CalcScreenPos(m_screenPos, m_enemyPos);
	m_spriteRender->SetPosition(m_screenPos);

	//残りHP量によってHPバーを短くする
	m_scale.x = (float)m_enemyHP / MAX_ENEMY_HP;

	//この敵がターゲットされているなら
	if (m_isTargeted) {
		//残りHP量にそったHPバーの長さに
		m_spriteRender->SetScale(m_scale);
	}
	else
	{
		//ターゲットされていないときは隠す
		m_spriteRender->SetScale(Vector3::Zero);
	}
}