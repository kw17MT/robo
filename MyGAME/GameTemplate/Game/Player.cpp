#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"
#include "MachinGun.h"
#include "CaptureStateManager.h"

bool Player::Start()
{
	//スキンを作成
	m_skinModelRender = NewGO<SkinModelRender>(0);
	//スキンの情報を初期化
	m_skinModelRender->Init("Assets/modelData/testBox/a12.tkm", "Assets/modelData/testBox/a12.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);



	//コリジョンの作成
	//m_skinModelRender->InitCharaCon(10.0f, 100.0f, m_currentPosition);
	m_charaCon.Init(10.0f, 100.0f, m_currentPosition);



	//最初のホームポジションを初期化
	m_currentHomePosition = m_currentPosition;
	//1フレーム前のホームポジションを初期化
	m_prevHomePosition = m_currentHomePosition;
	//モデルを初期位置に設定
	m_skinModelRender->SetPosition(m_currentHomePosition);

	m_animClip[0].Load("Assets/modelData/testBox/a12.tka");
	m_animClip[0].SetLoopFlag(true);
	m_skinModelRender->InitAnimation(m_animClip, 1);
	m_skinModelRender->PlayAnimation(1,1);

	//カメラの位置、ターゲットを初期化
	Vector3 cameraPos = m_currentHomePosition;
	cameraPos.y += 50.0f;
	g_camera3D->SetTarget(cameraPos);
	cameraPos.z += 230.0f;
	g_camera3D->SetPosition(cameraPos);

	return true;
}

void Player::Update()
{
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


	if (g_pad[0]->IsPress(enButtonRB2))
	{
		m_skinModelRender->PlayAnimation(1, 1);
		m_machingun.push_back(NewGO<MachinGun>(0));
		m_machingun.back()->SetPosition(m_currentPosition);
		m_machingun.back()->SetTargetPosition(CaptureStateManager::GetInstance().GetCapturedEnemyPos());
	}

	Vector3 a = { 1.0f,1.0f,1.0f };
	m_currentPosition = m_charaCon.Execute(a, 1);
	//m_charaCon.Execute({0.0f,})
}