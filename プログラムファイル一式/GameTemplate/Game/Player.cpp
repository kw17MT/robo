#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"
#include "MachinGun.h"
#include "CaptureStateManager.h"
#include "MissileGenerator.h"
#include "Reticle.h"
#include "PlayerEffect.h"
#include "GameDirector.h"
#include "SoundSource.h"

#include "PlayerEN.h"
#include "RestrictArea.h"

namespace
{
	const float CAMERA_POSITION_ADJUST_Y = 50.0f;
	const float CAMERA_POSITION_ADJUST_Z = 230.0f;

}

Player::~Player()
{
	DeleteGO(m_machingun);
	DeleteGO(m_skinModelRender);
	DeleteGO(m_missileGene);
	DeleteGO(m_playerHp);
	DeleteGO(m_playerEn);
	DeleteGO(m_area);
	DeleteGO(m_playerEffect);

	//個別で消す可能性があるためnullcheck
	if (m_reticle != nullptr)
	{
		DeleteGO(m_reticle);
	}
}

bool Player::Start()
{
	//スキンを作成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//スキンの情報を初期化
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", "Assets/modelData/robo/robo.tks", enModelUpAxisZ, true);

	m_animClip[enIdle].Load("Assets/animData/robo/idle1.tka");
	m_animClip[enIdle].SetLoopFlag(true);
	m_animClip[enFlying].Load("Assets/animData/robo/fly1.tka");
	m_animClip[enFlying].SetLoopFlag(true);
	m_animClip[enShooting].Load("Assets/animData/robo/shot1.tka");
	m_animClip[enShooting].SetLoopFlag(true);
	m_animClip[enBacking].Load("Assets/animData/robo/back1.tka");
	m_animClip[enBacking].SetLoopFlag(true);
	m_animClip[enUp].Load("Assets/animData/robo/up1.tka");
	m_animClip[enUp].SetLoopFlag(true);
	m_skinModelRender->InitAnimation(m_animClip, animNum);

	m_machingun = NewGO<MachinGun>(0);
	m_missileGene = NewGO<MissileGenerator>(0,"missileGene");

	m_reticle = NewGO<Reticle>(0);

	m_playerHp = NewGO<PlayerHP>(0);
	m_playerEn = NewGO<PlayerEN>(0);
	m_area = NewGO<RestrictArea>(0);
	m_playerEffect = NewGO<PlayerEffect>(0);

	//最初のホームポジションを初期化
	m_currentHomePosition = m_currentPosition;
	//1フレーム前のホームポジションを初期化
	m_prevHomePosition = m_currentHomePosition;
	//モデルを初期位置に設定
	m_skinModelRender->SetPosition(m_currentHomePosition);

	//カメラの位置、ターゲットを初期化
	Vector3 cameraPos = m_currentHomePosition;
	cameraPos.y += CAMERA_POSITION_ADJUST_Y;
	g_camera3D->SetTarget(cameraPos);
	cameraPos.z += CAMERA_POSITION_ADJUST_Z;
	g_camera3D->SetPosition(cameraPos);

	return true;
}

void Player::TakenDamage(EnPlayerDamageTypes damageType)
{
	m_playerHp->ApplyDamage(damageType);
}

void Player::Update()
{
	//プレイヤーのHPがなく、倒されていたら
	if (m_playerHp->GetIsPlayerAlive() == false)
	{
		m_playerEffect->SetBothBoosterPos(
			m_skinModelRender->GetBonePosition(L"BoosterR") + m_roboMove.GetMoveSpeed(),
			m_skinModelRender->GetBonePosition(L"BoosterL") + m_roboMove.GetMoveSpeed()
		);
		m_playerEffect->SetBothShoulderPos(
			m_skinModelRender->GetBonePosition(L"Bone044") + m_roboMove.GetMoveSpeed(),
			m_skinModelRender->GetBonePosition(L"Bone024") + m_roboMove.GetMoveSpeed()
		);

		if (GameDirector::GetInstance().GetGameScene() != enGameClear)
		{
			GameDirector::GetInstance().SetGameScene(enGameOver);
		}

		//レティクルだけ削除
		DeleteGO(m_reticle);


			CSoundSource* keepSE = FindGO<CSoundSource>("dashKeep");
			if (keepSE != nullptr)
			{
				DeleteGO(keepSE);
			}


		if (m_deathType == enAwayFromArea)
		{
			//一度だけ、エリア外に出た場所の斜め上にセットする
			m_cameraMove.SetDeadCamera(m_roboMove.GetMoveSpeed());
			//倒れた瞬間の1フレーム前の移動速度を用いて、そのまま移動させながら落ちる
			m_currentPosition = m_roboMove.DeadMove(m_currentPosition);
			m_skinModelRender->SetPosition(m_currentPosition);
			g_camera3D->SetTarget(m_currentPosition);
			m_machingun->SetPosition(m_skinModelRender->GetBonePosition(L"Bone046"),
				m_currentPosition,
				m_skinModelRender->GetBonePosition(L"Bone046") - m_skinModelRender->GetBonePosition(L"Bone045"));
			m_playerEffect->SetIsDied(true);
			return;
		}
		else
		{
			m_deathType = enByEnemy;
			m_cameraMove.SetIsDeadCamera(true);
			//倒された時、俯瞰気味でロボを見る
			m_cameraMove.SetDeadCamera(m_roboMove.GetMoveSpeed());

			m_currentPosition = m_roboMove.DeadMove(m_currentPosition);
			m_skinModelRender->SetPosition(m_currentPosition);
			g_camera3D->SetTarget(m_currentPosition);

			m_machingun->SetPosition(m_skinModelRender->GetBonePosition(L"Bone046"),
				m_currentPosition,
				m_skinModelRender->GetBonePosition(L"Bone046") - m_skinModelRender->GetBonePosition(L"Bone045"));
			m_playerEffect->SetIsDied(true);
			return;
		}
	}

	//プレイヤーのアニメーションを行う
	m_playerAnim.UpdateAnimState();
	m_skinModelRender->PlayAnimation(m_playerAnim.GetPlayerState(), 1.0f);

	m_roboMove.SetCanDash(m_playerEn->GetIsPlayerENRemain());

	//プレイヤーのホームポジションを更新
	m_currentHomePosition = m_roboMove.Execute(m_currentHomePosition);
	//ホームポジションによってカメラを動かす
	m_cameraMove.UpdatePlayerCamera(m_prevHomePosition, m_currentHomePosition);
	//右スティックのカメラの回転に従ったプレイヤーの回転
	m_roboRotation.UpdateRotation(m_skinModelRender);
	//計算し終わったホームポジションをもとにずらしたプレイヤーの位置を作成する。
	m_currentPosition = m_roboMove.CalcPlayerPos(m_currentHomePosition);
	//モデルを新しい位置に設定する。
	m_skinModelRender->SetPosition(m_currentPosition);
	//1フレーム前の座標として記録
	m_prevHomePosition = m_currentHomePosition;

	//プレイヤーの手の位置にマシンガンをセット
	m_machingun->SetPosition(m_skinModelRender->GetBonePosition(L"Bone046") + m_roboMove.GetMoveSpeed(),
		m_currentPosition,
		m_skinModelRender->GetBonePosition(L"Bone046") - m_skinModelRender->GetBonePosition(L"Bone045"));
	//マシンガンにターゲット位置とプレイヤーの現在の位置を与える
	if (m_reticle->GetIsTargeted()) {
		m_machingun->SetTargetPos(m_reticle->GetTargetingEnemyPos());
	}
	else
	{
		m_machingun->SetTargetPos(m_currentPosition + g_camera3D->GetForward() * 1000.0f);
	}
	m_missileGene->SetLaunchPos(m_currentPosition);

	m_playerEffect->SetBothBoosterPos(
		m_skinModelRender->GetBonePosition(L"BoosterR") + m_roboMove.GetMoveSpeed(),
		m_skinModelRender->GetBonePosition(L"BoosterL") + m_roboMove.GetMoveSpeed()
	);
	m_playerEffect->SetBothShoulderPos(
		m_skinModelRender->GetBonePosition(L"Bone044") + m_roboMove.GetMoveSpeed(),
		m_skinModelRender->GetBonePosition(L"Bone024") + m_roboMove.GetMoveSpeed()
	);
	m_playerEffect->CalcRotation(m_roboMove.GetMoveSpeed());
	m_playerEffect->SetIsDash(m_roboMove.GetIsDash());

	m_area->JudgeInArea(m_currentPosition);
	if (m_deathType == enStillAlive
		&& m_area->GetAreaType() == enDanger)
	{
		m_playerHp->SetHPZero();
		m_deathType = enAwayFromArea;
		m_cameraMove.SetIsDeadCamera(true);
	}



	if (m_area->GetAreaType() != enSafeArea)
	{
		if (m_playerHp->GetHPBarAmountInDanger() < m_area->GetMonochromeRateDependOnDistance())
		{
			RenderingEngine::GetInstance()->SetMonochromeRate(m_area->GetMonochromeRateDependOnDistance());
		}
		else
		{
			RenderingEngine::GetInstance()->SetMonochromeRate(m_playerHp->GetHPBarAmountInDanger() + 0.7f);
		}
	}
	else if(m_playerHp->GetHPBarAmountInDanger() != 0.0f)
	{
		RenderingEngine::GetInstance()->SetMonochromeRate(m_playerHp->GetHPBarAmountInDanger() + 0.7f);
	}
	else
	{
		RenderingEngine::GetInstance()->SetMonochromeRate(0.0f);
	}
}