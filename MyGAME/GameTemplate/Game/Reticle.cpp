#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"

namespace
{
	const Vector3 SCALE_CHANGE_AMOUNT = { 0.1f,0.1f,0.1f };
}

Reticle::~Reticle()
{
	DeleteGO(m_spriteRender[0]);
	DeleteGO(m_spriteRender[1]);
}

bool Reticle::Start()
{
	m_spriteRender[0] = NewGO<SpriteRender>(0);
	m_spriteRender[0]->Init("Assets/Image/reticle/reticle_idle.dds", 48, 48);
	m_spriteRender[0]->SetPosition(m_homePosition);

	m_spriteRender[1] = NewGO<SpriteRender>(0);
	m_spriteRender[1]->Init("Assets/Image/reticle/reticle_target.dds", 96, 96);
	m_spriteRender[1]->SetPosition(m_homePosition);

	return true;
}

void Reticle::CalcPosition()
{
	if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		//誰かしらを捕捉していないとロックオンできないように
		if (CaptureStateManager::GetInstance().GetCaptureState() == Captured)
		{
			m_isTarget = true;
			CaptureStateManager::GetInstance().SetCaptureState(Targeted);
		}
		//誰かしらをターゲットしている時
		else if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
		{
			m_isTarget = false;
			CaptureStateManager::GetInstance().SetCaptureState(None);
			CaptureStateManager::GetInstance().ResetNextEnemyParam();
		}
	}

	if (m_isTarget)
	{
		//レティクルの座標をターゲットされた敵の位置によって決定する。
		g_camera3D->CalcScreenPositionFromWorldPosition(
			m_lockOnPosition,
			CaptureStateManager::GetInstance().GetCapturedEnemyPos());

		Vector3 targetPos = Vector3::Zero;
		targetPos.x = -m_lockOnPosition.x;
		targetPos.y = m_lockOnPosition.y;

		Vector3 toCamera = g_camera3D->GetPosition() - m_lockOnPosition;
		//正規化
		toCamera.Normalize();
		//敵の位置とカメラの前方向の内積
		float dot = g_camera3D->GetForward().Dot(toCamera);
		//敵がカメラの前方向にあるならば映す
		if (dot < 0.0f)
		{
			targetPos.z = 0.0f;
		}
		//後ろ側にある
		else
		{
			targetPos.z = -1.0f;
			if (m_isTarget)
			{
				//ロックオンの対象が画面外に出たため、ロックオンを切る
				m_isTarget = false;
				CaptureStateManager::GetInstance().SetCaptureState(None);
			}
		}

		//ロックオン用の画像位置を設定
		m_spriteRender[1]->SetPosition(targetPos);

		//ロックオンしていたらスケールを1に戻していく
		m_reticleScale[1] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x >= 1.0f)
		{
			m_reticleScale[1] = Vector3::One;
		}
		//通常のレティクルは小さくしていく
		m_reticleScale[0] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x <= 0.0f)
		{
			m_reticleScale[0] = Vector3::Zero;
		}
	}
	//ターゲットしていなければ
	else
	{
		m_reticleScale[1] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[1].x <= 0)
		{
			m_reticleScale[1] = Vector3::Zero;
		}

		m_reticleScale[0] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[0].x >= 1)
		{
			m_reticleScale[0] = Vector3::One;
		}
	}
}

void Reticle::Update()
{
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		m_isTarget = false;
		Vector3 homePos = { 0.0f,0.0f,-1.0f };
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(homePos);
	}

	//ロックオンレティクルの位置、拡大率設定。通常レティクルの拡大率設定
	CalcPosition();

	m_spriteRender[0]->SetScale(m_reticleScale[0]);
	m_spriteRender[1]->SetScale(m_reticleScale[1]);
	m_spriteRender[0]->SetPosition(m_homePosition);
}