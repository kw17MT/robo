#include "stdafx.h"
#include "PlayerEffect.h"
#include "effect/Effect.h"
#include "GameDirector.h"
#include "SoundSource.h"

namespace
{
	const Vector3 BOOSTER_EFFECT_SCALE = { 0.4f,0.6f,0.4f };
}

PlayerEffect::~PlayerEffect()
{
	for (int i = 0; i < EFFECT_NUM; i++)
	{
		DeleteGO(m_effects[i]);
	}
}

bool PlayerEffect::Start()
{
	m_boosterScale = BOOSTER_EFFECT_SCALE;

	m_effects[0] = NewGO<Effect>(0);
	m_effects[0]->Init(u"Assets/effect/boost2.efk");
	m_boosterEffectRot.SetRotationX(90.0f);
	m_effects[0]->SetRotation(m_boosterEffectRot);
	m_effects[0]->SetScale(m_boosterScale);

	m_effects[1] = NewGO<Effect>(0);
	m_effects[1]->Init(u"Assets/effect/boost2.efk");
	m_effects[1]->SetRotation(m_boosterEffectRot);
	m_effects[1]->SetScale(m_boosterScale);

	m_effects[2] = NewGO<Effect>(0);
	m_effects[2]->Init(u"Assets/effect/boost2.efk");
	m_effects[2]->SetRotation(m_boosterEffectRot);
	m_effects[2]->SetScale(m_boosterScale);

	m_effects[3] = NewGO<Effect>(0);
	m_effects[3]->Init(u"Assets/effect/boost2.efk");
	m_effects[3]->SetRotation(m_boosterEffectRot);
	m_effects[3]->SetScale(m_boosterScale);
	return true;
}

void PlayerEffect::CalcRotation(Vector3 playerMoveDirection)
{
	Vector3 moveDir = playerMoveDirection;
	moveDir.Normalize();
	//エフェクトはYアップ
	m_boosterEffectRot.SetRotation(g_vec3AxisY, moveDir * -1.0f);
	m_aircontrailEffectRot.SetRotation(g_vec3AxisZ, moveDir);

	if (!g_pad[0]->GetLStickXF()
		&& GameDirector::GetInstance().GetGameScene() != enLaunchingPlayer)
	{
		m_boosterEffectRot.SetRotation(g_camera3D->GetRight(), 180.0f);
	}

	m_playerMoveSpeed = playerMoveDirection.Length();
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
		if (GameDirector::GetInstance().GetGameScene() == enLaunchingPlayer)
		{
			m_boosterScale = BOOSTER_EFFECT_SCALE * 3.0f;
		}
		else
		{
			m_boosterScale = BOOSTER_EFFECT_SCALE * 1.5f;
		}
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
		m_timeFromDeath += GameTime().GetFrameDeltaTime();
		if (!m_isExplodeNear)
		{
			CSoundSource* explodeNear = NewGO<CSoundSource>(0);
			explodeNear->Init(L"Assets/sound/roboExplodeNear.wav", false);
			explodeNear->SetVolume(1.0f);
			explodeNear->Play(false);

			Effect* explode = NewGO<Effect>(0);
			explode->Init(u"Assets/effect/roboExplode.efk");
			explode->SetPosition(m_boosterLeftPos);
			explode->SetScale({ 100.0f,100.0f,100.0f });
			explode->Play(false);

			Effect* dieEffect = NewGO<Effect>(0);
			dieEffect->Init(u"Assets/effect/explosion2.efk");
			dieEffect->SetScale({ 100.0f,100.0f,100.0f });
			dieEffect->Play();
			dieEffect->Update();
			dieEffect->SetPosition(m_boosterLeftPos);
			m_isExplodeNear = true;
		}

		if (m_timeFromDeath > 15.0f)
		{
			if (!m_isExplodeFar)
			{
				CSoundSource* explodeFar = NewGO<CSoundSource>(0);
				explodeFar->Init(L"Assets/sound/roboExplodeFar.wav", false);
				explodeFar->SetVolume(1.0f);
				explodeFar->Play(false);

				Effect* explode = NewGO<Effect>(0);
				explode->Init(u"Assets/effect/roboExplode.efk");
				explode->SetPosition(m_boosterLeftPos);
				explode->SetScale({ 100.0f,100.0f,100.0f });
				explode->Play(false);

				m_isExplodeFar = true;
			}

			if (m_timeFromDeath > 16.5f)
			{
				CSoundSource* explodeFar = NewGO<CSoundSource>(0);
				explodeFar->Init(L"Assets/sound/GameOver.wav", false);
				explodeFar->SetVolume(1.0f);
				explodeFar->Play(false);

				m_timeFromDeath = 0.0f;

				//最後の爆発を見せたのでシーンをゲームオーバー後にする
				GameDirector::GetInstance().SetGameScene(enAfterGameOver);
			}
		}

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

		m_effects[i]->SetRotation(m_boosterEffectRot);
		m_effects[i]->SetScale(m_boosterScale);
	}

	if (m_playerMoveSpeed > 3.0f)
	{
		Effect airContrail[2];
		airContrail[0].Init(u"Assets/effect/aircontrail1.efk");
		airContrail[1].Init(u"Assets/effect/aircontrail1.efk");
		airContrail[0].SetPosition(m_shoulderRightPos);
		airContrail[1].SetPosition(m_shoulderLeftPos);

		airContrail[0].SetScale({ 1.0f,1.0f,2.0f });
		airContrail[1].SetScale({ 1.0f,1.0f,2.0f });
		airContrail[0].SetRotation(m_aircontrailEffectRot);
		airContrail[1].SetRotation(m_aircontrailEffectRot);

		/*airContrail[0].Play();
		airContrail[1].Play();
		airContrail[0].Update();
		airContrail[1].Update();*/
	}
}