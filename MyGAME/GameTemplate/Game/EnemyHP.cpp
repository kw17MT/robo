#include "stdafx.h"
#include "EnemyHP.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_DAMAGE_AMOUNT = 10;			//マシンガンの弾のダメージ量
	const int MISSILE_DAMAGE_AMOUNT = 10;			//ミサイルのダメージ量
	const int RASER_DAMAGE_AMOUNT = 33;				//レールガンのダメージ量

	const float MAX_ENEMY_HP = 100.0f;
}

EnemyHP::~EnemyHP()
{
	//HPバーのインスタンス破棄
	DeleteGO(m_spriteRender);
}

bool EnemyHP::Start()
{
	//HPバーのモデル生成
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds",128,32);
	m_spriteRender->SetPivot({ 1.0f,1.5f });

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
		m_isDead = true;
	}
}

void EnemyHP::Update()
{
	//ワールド座標から、カメラの行列をつかってスクリーン座標に変換する
	Vector2 position;
	g_camera3D->CalcScreenPositionFromWorldPosition(position, m_enemyPos);
	m_screenPos.x = -position.x;
	m_screenPos.y = position.y;

	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//正規化
	enemyToCamera.Normalize();
	//敵の位置とカメラの前方向の内積
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//敵がカメラの前方向にあるならば映す
	if (dot < 0.0f)
	{
		m_screenPos.z = 0.0f;
	}
	//後ろ側にある
	else
	{
		m_screenPos.z = -1.0f;
	}
	m_spriteRender->SetPosition(m_screenPos);

	//残りHP量によってHPバーを短くする
	m_scale.x = (float)m_enemyHP / MAX_ENEMY_HP;

	//この敵がたーげとされているなら
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