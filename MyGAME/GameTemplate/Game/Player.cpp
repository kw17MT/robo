#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "CameraMove.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/robo/robo.tkm", nullptr, enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };

	m_homePosition = m_currentPosition;
	m_prevPosition = m_homePosition;

	m_skinModelRender->SetPosition(m_homePosition);
	Vector3 pos = m_homePosition;

	pos.y += 50.0f;
	g_camera3D->SetTarget(pos);
	pos.z += 230.0f;
	g_camera3D->SetPosition(pos);
	return true;
}

void Player::Update()
{
	//プレイヤーのホームポジションを更新
	m_homePosition = m_roboMove.Execute(m_homePosition);
	
	//ホームポジションによってカメラを動かす
	m_cameraMove.UpdatePlayerCamera(m_prevPosition, m_homePosition);

	//右スティックのカメラの回転に従ったプレイヤーの回転
	m_roboRotation.UpdateRotation(m_skinModelRender);

	m_currentPosition = m_roboMove.CalcPlayerPos(m_homePosition);
	m_skinModelRender->SetPosition(m_currentPosition);

	//更新した座標をモデルに適用
	//m_skinModelRender->SetPosition(m_homePosition);
	//1フレーム前の座標として記録
	//m_prevPosition = m_currentPosition;
	m_prevPosition = m_homePosition;
}