#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"

extern Vector3 CalcScreenPos(Vector3& screenPos, Vector3 pos);

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

void Reticle::ChangeTargetStateDependOnButtonLB2()
{
	//LB2ボタンを押している時間を計測する。
	if (g_pad[0]->IsPress(enButtonLB2))
	{
		m_PressButtonTime -= GameTime().GetFrameDeltaTime();
	}
	//LB2ボタンが押されていなければ
	if (!g_pad[0]->IsPress(enButtonLB2))
	{
		//ちょい押しぐらいならば通常ロックオン状態を変化させる。
		if (m_PressButtonTime <= 0.99f && m_PressButtonTime >= 0.5f)
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
		//LB2ボタンが離されたら押下時間をリセットする。
		m_PressButtonTime = 1.0f;
	}

	//長くLB2ボタンを押しているなら、ロケットのターゲティング開始
	if (m_PressButtonTime < 0.5f)
	{
		//押下時間が一定以上になればロケットのターゲット開始
		if (m_PressButtonTime <= 0.0f) {
			//ターゲットの最大数でなければ
			if (CaptureStateManager::GetInstance().GetRocketTargetNum() != 9) {
				CaptureStateManager::GetInstance().SetRocketTargetState(true);

				//何かをロックオンしたなら
				if(CaptureStateManager::GetInstance().GetIsRocketTargeted()){
				//ロケットターゲットを開始する時間に戻す
				m_PressButtonTime = 0.499f;
				CaptureStateManager::GetInstance().SetIsRocketTargeted(false);
				}
			}
		}
		else
		{
			//1フレームだけロケットのターゲットしたいのでターゲット状態を無しの状態に
			CaptureStateManager::GetInstance().SetRocketTargetState(false);
		}
	}
}

void Reticle::CalcPosition()
{
	if (m_isTarget)
	{
		//ターゲットする敵の位置をスクリーン座標に変換する。
		Vector3 targetPos = CalcScreenPos(m_lockOnPosition, CaptureStateManager::GetInstance().GetCapturedEnemyPos());
		//その位置がカメラの後ろ側に来るなら
		if (targetPos.z == -1.0f)
		{
			//ターゲットを解除
			m_isTarget = false;
			//プレイヤーのターゲット状態を解除する。
			CaptureStateManager::GetInstance().SetCaptureState(None);
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
	//プレイヤーのターゲット状態が何もなしならば
	if (CaptureStateManager::GetInstance().GetCaptureState() == None)
	{
		//レティクルはターゲットしていない
		m_isTarget = false;
		//それまでターゲットしていた敵の位置をリセットする。
		Vector3 homePos = { 0.0f,0.0f,-1.0f };
		CaptureStateManager::GetInstance().SetCapturedEnemyPos(homePos);
	}

	//LB2ボタンを押しているかでターゲット状態を設定する。
	ChangeTargetStateDependOnButtonLB2();

	//ロックオンレティクルの位置、拡大率設定。通常レティクルの拡大率設定
	CalcPosition();

	m_spriteRender[0]->SetScale(m_reticleScale[0]);
	m_spriteRender[1]->SetScale(m_reticleScale[1]);
	m_spriteRender[0]->SetPosition(m_homePosition);
}