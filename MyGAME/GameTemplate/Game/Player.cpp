#include "stdafx.h"
#include "Player.h"
#include "SkinModelRender.h"
#include "RoboMove.h"
#include "CameraMove.h"
#include "RoboRotation.h"

bool Player::Start()
{
	m_skinModelRender = NewGO<SkinModelRender>(0, "player");
	m_skinModelRender->Init("Assets/modelData/robo.tkm", "Assets/modelData/robo.tks", enModelUpAxisZ, { 0.0f,0.0f,0.0f }, true);
	m_currentPosition = { 0.0f,400.0f,5000.0f };
	m_skinModelRender->SetPosition(m_currentPosition);

	m_front = g_camera3D->GetForward();

	g_camera3D->SetTarget(m_currentPosition);
	return true;
}

void Player::Update()
{
	//���݂̃v���C���[�̃|�W�V�������X�V
	m_currentPosition = RoboMove::Move(m_currentPosition);
	m_currentFront = g_camera3D->GetForward();
	//�X�V�������W�����f���ɓK�p
	m_skinModelRender->SetPosition(m_currentPosition);
	//�v���C���[�����삷��J�����̓���
	CameraMove::UpdatePlayerCamera(m_prevPosition, m_currentPosition);

	//RoboRotation::UpdateRotation(m_skinModelRender, m_prevPosition, m_currentPosition, m_front);
	//m_skinModelRender->SetRotation(RoboRotation::RotationX(m_skinModelRender->GetRotation()));
	//�_�b�V��������Ȃ����
	/*if (!m_dash)
	{
		CameraMove::FollowPlayer(m_position);
	}*/

	//Quaternion qRot;
	//qRot.SetRotation(m_front, g_camera3D->GetUp() * -1.0f);
	//m_skinModelRender->SetRotation(qRot);

	//1�t���[���O�̍��W�Ƃ��ċL�^
	m_prevPosition = m_currentPosition;
	m_prevFront = m_currentFront;

	//Quaternion a;
	//a.AddRotationY(20);
	//m_skinModelRender->SetRotation(a);

	/*Quaternion a;
	a.SetRotation(m_prevFront, m_currentFront);
	a.Apply(m_prevFront);
	m_skinModelRender->SetRotation(a);*/

	m_roboLean.Lean(m_skinModelRender);
}