#include "stdafx.h"
#include "Reticle.h"
#include "CaptureStateManager.h"
#include "SoundSource.h"

extern void CalcMethods::CalcScreenPos(Vector3& screenPos, Vector3 pos);

namespace
{
	const Vector3 SCALE_CHANGE_AMOUNT = { 0.1f,0.1f,0.1f };
	const float SE_VOLUME = 1.0f;
}

Reticle::~Reticle()
{
	DeleteGO(m_spriteRender[enNormalReticle]);
	DeleteGO(m_spriteRender[enTargetReticle]);
}

bool Reticle::Start()
{
	m_spriteRender[enNormalReticle] = NewGO<SpriteRender>(0);
	m_spriteRender[enNormalReticle]->Init("Assets/Image/reticle/reticle_idle.dds", 20, 20);
	m_spriteRender[enNormalReticle]->SetPosition(m_homePosition);

	m_spriteRender[enTargetReticle] = NewGO<SpriteRender>(0);
	m_spriteRender[enTargetReticle]->Init("Assets/Image/reticle/reticle_target.dds", 96, 96);
	m_spriteRender[enTargetReticle]->SetPosition(m_homePosition);

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

				CSoundSource* selectSE = NewGO<CSoundSource>(0);
				selectSE->Init(L"Assets/sound/lockOn.wav", false);
				selectSE->SetVolume(SE_VOLUME);
				selectSE->Play(false);
			}
			//誰かしらをターゲットしている時
			else if (CaptureStateManager::GetInstance().GetCaptureState() == Targeted)
			{
				m_isTarget = false;
				CaptureStateManager::GetInstance().SetCaptureState(None);
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
			if (CaptureStateManager::GetInstance().GetMissileTargetState() != enFull) {
				//何かをロックオンしたなら
				if (CaptureStateManager::GetInstance().GetMissileTargetState() == enMissileTargeted)
				{
					//ロケットターゲットを開始する時間に戻す
					m_PressButtonTime = 0.499f;
					CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);

					CSoundSource* selectSE = NewGO<CSoundSource>(0);
					selectSE->Init(L"Assets/sound/missileLock.wav", false);
					selectSE->SetVolume(SE_VOLUME);
					selectSE->Play(false);
				}
				else
				{
					CaptureStateManager::GetInstance().SetMissileTargetState(enMissileTarget);
				}
			}
		}
		else
		{
			//1フレームだけロケットのターゲットしたいのでターゲット状態を無しの状態に
			CaptureStateManager::GetInstance().SetMissileTargetState(enNoTarget);
		}
	}
}

void Reticle::CalcPosition()
{
	if (m_isTarget)
	{
		//ターゲットする敵の位置をスクリーン座標に変換する。
		CalcMethods::CalcScreenPos(m_lockOnPosition, m_targetingEnemyPos);
		//その位置がカメラの後ろ側に来るなら
		if (m_lockOnPosition.z == -1.0f)
		{
			//ターゲットを解除
			m_isTarget = false;
			//プレイヤーのターゲット状態を解除する。
			CaptureStateManager::GetInstance().SetCaptureState(None);
		}

		//ロックオン用の画像位置を設定
		m_spriteRender[enTargetReticle]->SetPosition(m_lockOnPosition);

		//ロックオンしていたらスケールを1に戻していく
		m_reticleScale[enTargetReticle] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enTargetReticle].x >= 1.0f)
		{
			m_reticleScale[enTargetReticle] = Vector3::One;
		}
		//通常のレティクルは小さくしていく
		m_reticleScale[enNormalReticle] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enNormalReticle].x <= 0.0f)
		{
			m_reticleScale[enNormalReticle] = Vector3::Zero;
		}
	}
	//ターゲットしていなければ
	else
	{
		m_reticleScale[enTargetReticle] -= SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enTargetReticle].x <= 0)
		{
			m_reticleScale[enTargetReticle] = Vector3::Zero;
		}

		m_reticleScale[enNormalReticle] += SCALE_CHANGE_AMOUNT;
		if (m_reticleScale[enNormalReticle].x >= 1)
		{
			m_reticleScale[enNormalReticle] = Vector3::One;
		}
	}
}

void Reticle::Update()
{
	//プレイヤーのターゲット状態が何もなしならば
	if (CaptureStateManager::GetInstance().GetCaptureState() != Targeted)
	{
		//レティクルはターゲットしていない
		m_isTarget = false;
	}

	if (CaptureStateManager::GetInstance().GetCaptureState() == ChangeMainTarget
		&& !CaptureStateManager::GetInstance().GetIsDecidedNextTarget())
	{
		CaptureStateManager::GetInstance().SetCaptureState(None);
	}

	//LB2ボタンを押しているかでターゲット状態を設定する。
	ChangeTargetStateDependOnButtonLB2();

	//ロックオンレティクルの位置、拡大率設定。通常レティクルの拡大率設定
	CalcPosition();

	m_spriteRender[enNormalReticle]->SetScale(m_reticleScale[enNormalReticle]);
	m_spriteRender[enTargetReticle]->SetScale(m_reticleScale[enTargetReticle]);
	m_spriteRender[enNormalReticle]->SetPosition(m_homePosition);

	m_targetReticleRotateRate += 1.0f;
	Quaternion targetReticleRot;
	targetReticleRot.SetRotationDegZ(m_targetReticleRotateRate);
	m_spriteRender[enTargetReticle]->SetRotation(targetReticleRot);
}