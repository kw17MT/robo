#include "stdafx.h"
#include "PlayerEffect.h"
#include "effect/Effect.h"

namespace
{
	const Vector3 BOOSTER_EFFECT_SCALE = { 0.4f,0.4f,0.4f };
}

PlayerEffect::~PlayerEffect()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		DeleteGO(m_effects[i]);
	}
	DeleteGO(m_dieEffect);
}

bool PlayerEffect::Start()
{
	m_boosterScale = BOOSTER_EFFECT_SCALE;

	m_effects[0] = NewGO<Effect>(0);
	m_effects[0]->Init(u"Assets/effect/boost2.efk");
	m_effectRot.SetRotationX(90.0f);
	m_effects[0]->SetRotation(m_effectRot);
	m_effects[0]->SetScale(m_boosterScale);

	m_effects[1] = NewGO<Effect>(0);
	m_effects[1]->Init(u"Assets/effect/boost2.efk");
	m_effects[1]->SetRotation(m_effectRot);
	m_effects[1]->SetScale(m_boosterScale);

	m_effects[2] = NewGO<Effect>(0);
	m_effects[2]->Init(u"Assets/effect/boost2.efk");
	m_effects[2]->SetRotation(m_effectRot);
	m_effects[2]->SetScale(m_boosterScale);

	m_effects[3] = NewGO<Effect>(0);
	m_effects[3]->Init(u"Assets/effect/boost2.efk");
	m_effects[3]->SetRotation(m_effectRot);
	m_effects[3]->SetScale(m_boosterScale);

	m_dieEffect = NewGO<Effect>(0);
	m_dieEffect->Init(u"Assets/effect/explosion2.efk");
	m_dieEffect->SetScale({ 100.0f,100.0f,100.0f });


	return true;
}

void PlayerEffect::CalcRotation(Vector3 playerMoveDirection)
{
	Vector3 moveDir = playerMoveDirection;
	moveDir.Normalize();
	//エフェクトはYアップ
	m_effectRot.SetRotation(g_vec3AxisY, moveDir * -1.0f);

	if (!g_pad[0]->GetLStickXF())
	{
		m_effectRot.SetRotation(g_camera3D->GetRight(), 180.0f);
	}
}

void PlayerEffect::Update()
{
	m_effectTimer++;
	if (m_effectTimer == BOOSTER_EFFECT_FRAME)
	{
		m_effects[0]->Play();
		m_effects[1]->Play();
	}
	else if (m_effectTimer == BOOSTER_EFFECT_FRAME * 2)
	{
		m_effects[2]->Play();
		m_effects[3]->Play();
		m_effectTimer = 0;
	}

	if (m_isDash)
	{
		m_boosterScale = BOOSTER_EFFECT_SCALE * 1.5f;
		m_isDash = false;
	}



	m_boosterScale.x -= GameTime().GetFrameDeltaTime() / 2.0f;
	m_boosterScale.y -= GameTime().GetFrameDeltaTime() / 2.0f;
	m_boosterScale.z -= GameTime().GetFrameDeltaTime() / 2.0f;

	if (m_boosterScale.x <= BOOSTER_EFFECT_SCALE.x)
	{
		m_boosterScale = BOOSTER_EFFECT_SCALE;
	}

	if (m_isPlayerDied)
	{
		m_dieEffect->Play();
		m_dieEffect->Update();
		m_dieEffect->SetPosition(m_boosterLeftPos);
	}


	for (int i = 0; i < EFFECT_NUM; i++)
	{

		if (i % 2 == 0)
		{
			m_effects[i]->SetPosition(m_boosterRightPos);
		}
		else
		{
			m_effects[i]->SetPosition(m_boosterLeftPos);
		}

		m_effects[i]->SetRotation(m_effectRot);
		m_effects[i]->SetScale(m_boosterScale);
	}
}