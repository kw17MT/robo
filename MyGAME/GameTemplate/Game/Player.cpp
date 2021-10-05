#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "RoboMove.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0, "player");
	m_skinModelRender->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };
	m_skinModelRender->SetPosition(m_currentPosition);
	

	g_camera3D->SetTarget(m_currentPosition);
	return true;
}

void Player::Update()
{
	//現在のプレイヤーのポジションを更新
	m_currentPosition = RoboMove::Move(m_currentPosition);
	//更新した座標をモデルに適用
	m_skinModelRender->SetPosition(m_currentPosition);
	//プレイヤーが操作するカメラの動き
	CameraMove::UpdatePlayerCamera(m_prevPosition, m_currentPosition);;
	
	//ダッシュ中じゃなければ
	/*if (!m_dash)
	{
		CameraMove::FollowPlayer(m_position);
	}*/

	//1フレーム前の座標として記録
	m_prevPosition = m_currentPosition;
}