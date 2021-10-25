#include "stdafx.h"
#include "EnemyHP.h"
#include "SpriteRender.h"

namespace
{
	const int BULLET_DAMAGE_AMOUNT = 10;
	const int MISSILE_DAMAGE_AMOUNT = 10;
	const int RASER_DAMAGE_AMOUNT = 33;
}

EnemyHP::~EnemyHP()
{
	DeleteGO(m_spriteRender);
}

bool EnemyHP::Start()
{
	m_spriteRender = NewGO<SpriteRender>(0);
	m_spriteRender->Init("Assets/Image/HPBar/HPBar.dds",128,32);
	m_spriteRender->SetPivot({ 1.0f,1.5f });

	return true;
}

void EnemyHP::ApplyDamage(EnDamageTypes damageType)
{
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

	if (m_enemyHP <= 0.0f)
	{
		m_isDead = true;
	}
}

void EnemyHP::Update()
{
	Vector2 position;
	g_camera3D->CalcScreenPositionFromWorldPosition(position, m_enemyPos);
	m_position.x = -position.x;
	m_position.y = position.y;

	Vector3 enemyToCamera = g_camera3D->GetPosition() - m_enemyPos;
	//³‹K‰»
	enemyToCamera.Normalize();
	//“G‚ÌˆÊ’u‚ÆƒJƒƒ‰‚Ì‘O•ûŒü‚Ì“àÏ
	float dot = g_camera3D->GetForward().Dot(enemyToCamera);
	//“G‚ªƒJƒƒ‰‚Ì‘O•ûŒü‚É‚ ‚é‚È‚ç‚Î‰f‚·
	if (dot < 0.0f)
	{
		m_position.z = 0.0f;
	}
	//Œã‚ë‘¤‚É‚ ‚é
	else
	{
		m_position.z = -1.0f;
	}
	m_spriteRender->SetPosition(m_position);

	m_scale.x = (float)m_enemyHP / 100.0f;

	if (m_isTargeted) {
		m_spriteRender->SetScale(m_scale);
	}
	else
	{
		m_spriteRender->SetScale(Vector3::Zero);
	}
}